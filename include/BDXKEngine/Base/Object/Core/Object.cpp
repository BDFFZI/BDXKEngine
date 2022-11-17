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

    ObjectPtrBase Object::Instantiate(const ObjectPtrBase& objectPtr)
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

        return objectPtr;
    }
    ObjectPtrBase Object::Instantiate(const ObjectPtrBase& objectPtr, Serializer& serializer)
    {
        if (objectPtr == nullptr) throw std::exception("实例化的物体为空");

        //克隆并注册
        const auto instance = dynamic_cast<Object*>(serializer.Clone(objectPtr.ToObjectBase()));
        instance->name = instance->name + " (Clone)";
        return Instantiate(instance);
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

    std::string Object::ToString()
    {
        std::stringstream stream;
        stream << "编号：" << instanceID << std::endl;
        stream << "运行中：" << isRunning << std::endl;
        return stream.str();
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
