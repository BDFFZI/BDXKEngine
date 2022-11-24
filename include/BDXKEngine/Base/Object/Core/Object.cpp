#include "Object.h"
#include <iostream>
#include <sstream>
#include "ObjectPtrTransferer.h"

namespace BDXKEngine
{
    int Object::instanceIDCount = 0;
    std::map<int, Object*> Object::allObjects = {};

    std::map<int, Object*> Object::GetAllObjects()
    {
        return allObjects;
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
        if (objectPtr == nullptr) throw std::exception("实例化的物体为空");

        //获取关联物体
        ObjectPtrTransferer referenceTransferer = {objectPtr.GetInstanceID()};
        objectPtr->Transfer(referenceTransferer);
        auto references = referenceTransferer.GetReferences();

        //激活
        for (auto reference = references.rbegin(); reference != references.rend(); ++reference)
        {
            Object* referenceObject = FindObjectOfInstanceID(*reference);
            if (referenceObject != nullptr && referenceObject->isRunning == false)
            {
                referenceObject->isRunning = true;
                referenceObject->Awake();
            }
        }
    }
    ObjectPtrBase Object::Clone(const ObjectPtrBase& objectPtr, Serializer& serializer, bool instantiate)
    {
        if (objectPtr == nullptr) throw std::exception("实例化的物体为空");

        //克隆并注册
        ObjectPtrBase instance = dynamic_cast<Object*>(serializer.Clone(objectPtr.ToObjectBase()));
        instance->name = instance->name + " (Clone)";
        if (instantiate)Instantiate(instance);
        return instance;
    }
    void Object::DestroyImmediate(const ObjectPtrBase& objectPtr)
    {
        Object* object = objectPtr.ToObjectBase();
        if (object == nullptr || object->isDestroying)
            return;

        if (object->isRunning == false) //根本就没被唤醒过，所以也不需要调用销毁回调
        {
            delete object;
            return;
        }

        object->isDestroying = true;
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
