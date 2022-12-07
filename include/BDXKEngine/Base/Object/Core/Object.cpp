#include "Object.h"
#include <iostream>
#include <sstream>
#include "ObjectPtrTransferer.h"

namespace BDXKEngine
{
    ObjectPtrBase Object::Create(const Type& type)
    {
        const auto object = dynamic_cast<Object*>(Reflection::GetReflection(type).GetConstruction());
        if (object == nullptr)throw std::exception("目标未Object类型");
        ObjectPtrBase window = object;
        Instantiate(window);
        return window;
    }
    const std::map<int, Object*>& Object::GetObjects()
    {
        return allObjects;
    }
    void Object::ReplaceObject(ObjectPtrBase& source, int targetInstanceID)
    {
        Object* object = source.ToObjectBase();

        DestroyImmediate(FindObjectOfInstanceID(targetInstanceID));
        allObjects[targetInstanceID] = object;
        allObjects.erase(object->instanceID);
        object->instanceID = targetInstanceID;

        source = {object};
    }
    Object::Object()
    {
        instanceID = ++instanceIDCount;
        allObjects[instanceID] = this;
    }
    Object::~Object()
    {
        allObjects.erase(instanceID);
    }

    // ReSharper disable once CppParameterMayBeConstPtrOrRef 禁止设置为常量，因为这可能导致填入参数为右值，从而错误触发安全指针的回收功能
    void Object::Instantiate(ObjectPtrBase& objectPtr)
    {
        if (objectPtr.IsNull()) throw std::exception("实例化的物体为空");
        if (objectPtr->name.empty()) objectPtr->name = objectPtr->GetType();

        //获取关联物体
        ObjectPtrTransferer referenceTransferer = {objectPtr};
        objectPtr->Transfer(referenceTransferer);
        auto references = referenceTransferer.GetReferences();

        //激活
        for (auto reference = references.rbegin(); reference != references.rend(); ++reference)
        {
            Object* referenceObject = FindObjectOfInstanceID(*reference);
            if (referenceObject != nullptr && referenceObject->isInstantiated == false)
            {
                referenceObject->isInstantiated = true;
                referenceObject->Awake();
            }
        }
    }
    ObjectPtrBase Object::Clone(const ObjectPtrBase& objectPtr, Serializer& serializer, bool instantiate)
    {
        if (objectPtr.IsNull()) throw std::exception("实例化的物体为空");

        //克隆并注册
        ObjectPtrBase instance = dynamic_cast<Object*>(serializer.Clone(objectPtr.ToObjectBase()));
        instance->name = instance->name;
        if (instantiate)Instantiate(instance);
        return instance;
    }
    void Object::DestroyImmediate(const ObjectPtrBase& objectPtr)
    {
        Object* object = objectPtr.ToObjectBase();
        if (object == nullptr || object->isDestroyed)
            return;

        if (object->isInstantiated == false) //根本就没被唤醒过，所以也不需要调用销毁回调
        {
            delete object;
            return;
        }

        object->isDestroyed = true;
        object->Destroy();
        delete object;
    }

    Object* Object::FindObjectOfInstanceID(const int instanceID)
    {
        if (const auto pair = allObjects.find(instanceID); pair != allObjects.end())
            return pair->second;

        return nullptr;
    }

    int Object::GetInstanceID() const
    {
        return instanceID;
    }
    std::string Object::GetName() const
    {
        return name;
    }

    void Object::SetName(const std::string& name)
    {
        this->name = name;
    }

    bool Object::IsInstantiated() const
    {
        return isInstantiated;
    }
    bool Object::IsDestroyed() const
    {
        return isDestroyed;
    }

    void Object::Transfer(Transferer& transferer)
    {
        Reflective::Transfer(transferer);

        TransferFieldInfo(name);
    }

    void Object::Awake()
    {
        std::cout << "Object::Awake " + std::to_string(instanceID) + " " + name << std::endl;
    }
    void Object::Destroy()
    {
        std::cout << "Object::Destroy " + std::to_string(instanceID) + " " + name << std::endl;
    }
}
