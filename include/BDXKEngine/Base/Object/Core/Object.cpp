#include "Object.h"
#include <iostream>
#include <sstream>

namespace BDXKEngine
{
    class ReferenceTransferer : public Transferer
    {
    public:
        const std::vector<ObjectPtrBase>& GetReferences()
        {
            return references;
        }
    protected:
        void TransferValue(void* value, const Type& type) override
        {
            if (type.find("ObjectPtr") != std::string::npos)
            {
                references.push_back(*static_cast<ObjectPtrBase*>(value));
            }
        }
    private:
        std::vector<ObjectPtrBase> references;
    };

    int Object::instanceIDCount = 0;
    std::map<int, Object*> Object::allObjects = {};

    std::map<int, Object*> Object::GetAllObjects()
    {
        return allObjects;
    }
    void Object::InstantiateNoAwake(Object* object)
    {
        if (object == nullptr) throw std::exception("实例化的物体为空");
        if (object->instanceID != 0) throw std::exception("物体已被实例化");

        //注册
        object->instanceID = ++instanceIDCount;
        allObjects[object->instanceID] = object;
    }
    void Object::Instantiate(Object* object)
    {
        InstantiateNoAwake(object);

        //获取关联物体
        ReferenceTransferer referenceTransferer;
        object->Transfer(referenceTransferer);
        auto& references = referenceTransferer.GetReferences();

        //激活
        for (auto reference = references.rbegin(); reference != references.rend(); ++reference)
        {
            Object* referenceObject = reference->ToObjectBase();
            if (referenceObject != nullptr && referenceObject->isRunning == false)
            {
                referenceObject->isRunning = true;
                referenceObject->Awake();
            }
        }
        object->isRunning = true;
        object->Awake();
    }
    ObjectPtrBase Object::InstantiateNoAwake(const ObjectPtrBase& objectPtr, Serializer& serializer)
    {
        if (objectPtr == nullptr) throw std::exception("实例化的物体为空");

        //克隆并注册
        const auto instance = dynamic_cast<Object*>(serializer.Clone(objectPtr.ToObjectBase()));
        instance->name = instance->name + " (Clone)";
        InstantiateNoAwake(instance);
        return instance;
    }
    ObjectPtrBase Object::Instantiate(const ObjectPtrBase& objectPtr, Serializer& serializer)
    {
        if (objectPtr == nullptr) throw std::exception("实例化的物体为空");

        //克隆并注册
        const auto instance = dynamic_cast<Object*>(serializer.Clone(objectPtr.ToObjectBase()));
        instance->name = instance->name + " (Clone)";
        Instantiate(instance);
        return instance;
    }
    void Object::DestroyImmediate(const ObjectPtrBase& objectPtr)
    {
        Object* object = objectPtr.ToObjectBase();
        if (object == nullptr || object->isDestroying)
            return;

        if (object->isRunning == false) //根本就没被唤醒过，所以也不需要调用销毁回调
        {
            allObjects.erase(object->instanceID);
            delete object;
            return;
        }

        object->isDestroying = true;
        object->Destroy();
        allObjects.erase(object->instanceID);
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
    bool Object::IsRunning() const
    {
        return isRunning;
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
