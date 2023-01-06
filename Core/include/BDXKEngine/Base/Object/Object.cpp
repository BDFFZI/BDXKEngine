#include "Object.h"
#include <iostream>
#include <sstream>
#include "Transferer/ObjectPtrTransferer.h"

namespace BDXKEngine
{
    const std::map<int, Object*>& Object::GetObjects()
    {
        return allObjects;
    }
    void Object::ReplaceObject(ObjectPtrBase& source, int targetInstanceID)
    {
        Object* object = source.ToObjectBase();
        if (object == nullptr)
            throw std::exception("替换物体不存在");
        if (object->GetInstanceID() == targetInstanceID)
            throw std::exception("新旧ID相同");
        if (targetInstanceID <= 0)
            throw std::exception("目标ID不可使用");

        DestroyImmediate(FindObjectOfInstanceID(targetInstanceID));
        allObjects[targetInstanceID] = object;
        allObjects.erase(object->instanceID);
        object->instanceID = targetInstanceID;

        source = {object}; //source.instanceID已从allObjects移除，所以替换时并不会导致错误删除
    }
    void Object::SetConstructedObjectEvent(const std::function<void(Object*)>& onConstructedObject)
    {
        Object::onConstructedObject = onConstructedObject;
    }
    ObjectPtrBase Object::Create(const Type& type)
    {
        const auto object = dynamic_cast<Object*>(Reflection::GetReflection(type).GetConstruction());
        if (object == nullptr)throw std::exception("目标未Object类型");
        ObjectPtrBase window = object;
        Instantiate(window);
        return window;
    }
    Object::Object()
    {
        instanceID = ++instanceIDCount;
        allObjects[instanceID] = this;

        if (onConstructedObject != nullptr)onConstructedObject(this);
    }
    Object::~Object()
    {
        if (instanceID == 88)
            printf("");

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
        std::vector<int> awakeObjects;
        for (auto reference = references.begin(); reference != references.end(); ++reference)
        {
            Object* referenceObject = FindObjectOfInstanceID(*reference);
            if (referenceObject != nullptr && referenceObject->isInstantiated == false)
            {
                referenceObject->isInstantiated = true;
                referenceObject->Awake();
                awakeObjects.push_back(*reference);
            }
        }
        for (const auto instanceID : awakeObjects)
        {
            Object* referenceObject = FindObjectOfInstanceID(instanceID);
            if (referenceObject != nullptr)referenceObject->PostAwake();
        }
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
        object->PreDestroy(); //利用Transfer我可以知道实例化一个物体时需要连带实例化的其他物体，但我无法知道删除一个物体需要连带删除哪些，所以PreDestroy目前名存实亡
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
    const std::string& Object::GetName() const
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
        //std::cout << "Object::Awake " + std::to_string(instanceID) + " " + name << std::endl;
    }
    void Object::Destroy()
    {
        //std::cout << "Object::Destroy " + std::to_string(instanceID) + " " + name << std::endl;
    }
}
