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

    std::map<int, Object*> Object::GetAllObjects()
    {
        return allObjects;
    }
    ObjectPtrBase Object::InstantiateNoAwake(Object* object)
    {
        if (object == nullptr) throw std::exception("实例化的物体为空");
        if (object->instanceID != 0) throw std::exception("物体已被实例化");

        //注册
        object->instanceID = ++instanceIDCount;
        allObjects[object->instanceID] = object;

        return object;
    }
    ObjectPtrBase Object::Instantiate(Object* object)
    {
        ObjectPtrBase objectPtr = InstantiateNoAwake(object);

        //激活
        const std::vector<Object*> lastAwakeQueue = postAwakeQueue;
        postAwakeQueue.clear();
        MarkAwake(objectPtr);
        FlushAwakeQueue();
        postAwakeQueue = lastAwakeQueue;

        return objectPtr;
    }
    ObjectPtrBase Object::InstantiateNoAwake(const ObjectPtrBase& objectPtr, Serializer& serializer)
    {
        if (objectPtr == nullptr) throw std::exception("实例化的物体为空");

        //克隆并注册
        const auto instance = dynamic_cast<Object*>(serializer.Clone(objectPtr.ToObjectBase()));
        instance->instanceID = ++instanceIDCount;
        instance->name = instance->name + " (Clone)";
        allObjects[instance->instanceID] = instance;

        return instance;
    }
    ObjectPtrBase Object::Instantiate(const ObjectPtrBase& objectPtr, Serializer& serializer)
    {
        const ObjectPtrBase objectPtrBase = InstantiateNoAwake(objectPtr, serializer);

        //激活
        const std::vector<Object*> lastAwakeQueue = postAwakeQueue;
        postAwakeQueue.clear();
        MarkAwake(objectPtrBase);
        FlushAwakeQueue();
        postAwakeQueue = lastAwakeQueue;

        return objectPtrBase;
    }
    void Object::DestroyImmediate(const ObjectPtrBase& object)
    {
        const std::vector<Object*> lastDestroyQueue = postDestroyQueue;
        postDestroyQueue.clear();
        MarkDestroy(object);
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

    void Object::MarkAwake(const ObjectPtrBase& objectPtr)
    {
        Object* object = objectPtr.ToObjectBase();
        if (object == nullptr || object->isRunning)
            return;

        allObjectsRunning[object->instanceID] = object;
        object->isRunning = true;
        object->MarkAwake();

        postAwakeQueue.push_back(object);
    }
    void Object::MarkDestroy(const ObjectPtrBase& objectPtr)
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
        object->MarkDestroy();
        allObjectsRunning.erase(object->instanceID);

        postDestroyQueue.push_back(object);
    }

    void Object::MarkAwake()
    {
    }
    void Object::MarkDestroy()
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
