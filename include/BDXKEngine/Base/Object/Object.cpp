#include "Object.h"
#include "BDXKEngine/Base/Serialization/Binary/BinaryReader.h"

namespace BDXKEngine
{
    int Object::instanceIDCount = 0;
    std::unordered_map<int, Object*> Object::allObjects = {};
    std::unordered_map<int, Object*> Object::allObjectsRunning = {};
    std::vector<Object*> Object::postDestroyQueue;
    std::vector<Object*> Object::postAwakeQueue;

    std::unordered_map<std::string, std::function<Object*()>> Object::serializationInfo = {};

    void Object::DebugObjectCount()
    {
        Debug::LogWarning("当前所有Object数量:" + std::to_string(allObjects.size()));
    }

    Object* Object::InstantiateNoAwake(Object* serializer)
    {
        if (serializer == nullptr)
            Debug::LogException("实例化的物体为空");

        //取出类型信息
        std::stringstream stream = {};
        BinaryWriter exporter = {stream};
        serializer->Transfer(exporter);
        BinaryReader importer = {stream};
        std::string typenameTemp;
        importer.TransferField("typename", typenameTemp);
        if (typenameTemp.empty())
            Debug::LogException("无法获取类型ID，检查Transfer(Transferer&)的重写是否规范");

        //重新取出数据
        stream.str("");
        serializer->Transfer(exporter);
        //创建实例
        const auto constructor = serializationInfo.find(typenameTemp);
        if (constructor == serializationInfo.end())
            Debug::LogException("该类型(" + typenameTemp + ")的序列化信息未被注册");
        Object* result = constructor->second();
        result->Transfer(importer);
        return result;
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
        std::string serializationID = ParseTypeID(this);

        if (transferrer.GetTransferDirection() != TransferDirection::Inspect)
            transferrer.TransferField("serializationID", serializationID);

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
