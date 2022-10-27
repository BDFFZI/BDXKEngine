#include "Object.h"
#include "BDXKEngine/Base/Serialization/Binary/BinaryReader.h"

namespace BDXKEngine
{
    int Object::instanceIDCount = 0;
    std::unordered_map<int, Object*> Object::allObjects = {};
    std::unordered_map<int, Object*> Object::allObjectsRunning = {};
    std::vector<Object*> Object::postDestroyQueue;
    std::vector<Object*> Object::postAwakeQueue;

    std::unordered_map<std::string, std::function<Object*()>> Object::creators = {};

    void Object::DebugObjectCount()
    {
        Debug::LogWarning("当前所有Object数量:" + std::to_string(allObjects.size()));
    }
    Object* Object::GetCreator(const std::string& typeID)
    {
        const auto constructor = creators.find(typeID);
        return constructor == creators.end() ? nullptr : constructor->second();
    }

    Object* Object::InstantiateNoAwake(Object* serializer)
    {
        if (serializer == nullptr)
            Debug::LogException("实例化的物体为空");

        //创建实例
        const std::string typeID = serializer->GetTypeID();
        Object* creator = GetCreator(typeID);
        if (creator == nullptr)
            Debug::LogException("该类型(" + typeID + ")的创建器未被注册");
        //填充数据
        std::stringstream stream = {};
        BinaryWriter exporter = {stream};
        serializer->Transfer(exporter);
        BinaryReader importer = {stream};
        creator->Transfer(importer);
        return creator;
    }

    void Object::DestroyImmediate(Object* object)
    {
        const std::vector<Object*> lastDestroyQueue = postDestroyQueue;
        postDestroyQueue.clear();
        Destroy(object);
        FlushDestroyQueue();
        postDestroyQueue = lastDestroyQueue;
    }

    void Object::Awake(Object* object)
    {
        if (object == nullptr || object->isRunning)
            return;

        allObjectsRunning[object->instanceID] = object;
        object->isRunning = true;
        object->PreAwake();

        postAwakeQueue.push_back(object);
    }

    void Object::Destroy(Object* object)
    {
        if (object == nullptr || object->isDestroying)
            return;
        if (object->isRunning == false)
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


    Object* Object::FindObjectOfInstanceID(const int instanceID)
    {
        if (const auto pair = allObjects.find(instanceID); pair != allObjects.end())
            return pair->second;

        return nullptr;
    }
    std::string Object::GetTypeID() const
    {
        return std::string(typeid(*this).name()).substr(6);
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

    std::string Object::ToString()
    {
        std::stringstream stream;
        stream << "编号：" << instanceID << std::endl;
        stream << "运行中：" << isRunning << std::endl;
        return stream.str();
    }

    void Object::Transfer(Transferrer& transferrer)
    {
        std::string typeID = GetTypeID();
        transferrer.TransferField("typeID", typeID);
        std::string nameTemp = name;
        transferrer.TransferField("name", nameTemp);

        if (nameTemp.empty() == false) name = nameTemp;
    }
    void Object::PreAwake()
    {
    }
    void Object::PreDestroy()
    {
    }
    void Object::Awake()
    {
        Debug::Log("Object::Awake " + std::to_string(instanceID) + " " + name, 1);
    }
    void Object::Destroy()
    {
        Debug::Log("Object::Destroy " + std::to_string(instanceID) + " " + name, 1);
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
