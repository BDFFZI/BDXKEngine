#include "Object.h"
#include <iostream>
#include <sstream>

namespace BDXKEngine
{
    int Object::instanceIDCount = 0;
    std::map<int, Object*> Object::allObjects = {};
    std::map<int, Object*> Object::allObjectsRunning = {};
    std::vector<Object*> Object::postDestroyQueue;
    std::vector<Object*> Object::postAwakeQueue;

    Object* Object::InstantiateNoAwake(Object* object, Serializer& serializer)
    {
        if (object == nullptr) throw std::exception("实例化的物体为空");

        //克隆数据
        const auto instance = dynamic_cast<Object*>(serializer.Clone(object));
        instance->instanceID = ++instanceIDCount;
        instance->name = instance->GetTypeID();
        allObjects[instance->instanceID] = instance;

        return instance;
    }
    void Object::DestroyImmediate(Object* object)
    {
        const std::vector<Object*> lastDestroyQueue = postDestroyQueue;
        postDestroyQueue.clear();
        PreDestroy(object);
        FlushDestroyQueue();
        postDestroyQueue = lastDestroyQueue;
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
    bool Object::GetIsRunning() const
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

    void Object::PreAwake(Object* object)
    {
        if (object == nullptr || object->isRunning)
            return;

        allObjectsRunning[object->instanceID] = object;
        object->isRunning = true;
        object->PreAwake();

        postAwakeQueue.push_back(object);
    }
    void Object::PreDestroy(Object* object)
    {
        if (object == nullptr || object->isDestroying)
            return;

        if (object->isRunning == false) //根本就没被唤醒过，所以也不需要调用销毁回调
        {
            allObjects.erase(object->instanceID);
            delete object;
            return;
        }

        object->isDestroying = true;
        object->PreDestroy();
        allObjectsRunning.erase(object->instanceID);

        postDestroyQueue.push_back(object);
    }

    void Object::PreAwake()
    {
    }
    void Object::PreDestroy()
    {
    }
    void Object::Awake()
    {
        std::cout << "Object::Awake " + std::to_string(instanceID) + " " + name << std::endl;
    }
    void Object::Destroy()
    {
        std::cout << "Object::Destroy " + std::to_string(instanceID) + " " + name << std::endl;
    }

    void Object::FlushAwakeQueue()
    {
        for (const auto& object : postAwakeQueue)
        {
            object->Awake();
        }
        postAwakeQueue.clear();
    }
    void Object::FlushDestroyQueue()
    {
        for (const auto& object : postDestroyQueue)
        {
            object->Destroy();
        }
        for (const auto& object : postDestroyQueue)
        {
            allObjects.erase(object->instanceID);
            delete object;
        }
        postDestroyQueue.clear();
    }
}
