#include "Object.h"

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
        //取出类型信息
        std::stringstream stream = {};
        BinaryExporter exporter = {stream};
        serializer->Transfer(exporter);
        BinaryImporter importer = {stream};
        std::string typenameTemp;
        importer.TransferString("typename", typenameTemp);
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
        object->Awake();

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
        object->Destroy();
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

    std::string Object::GetName()
    {
        return name;
    }

    void Object::SetName(const std::string& name)
    {
        this->name = name;
    }

    bool Object::GetIsRunning() const
    {
        return isRunning;
    }

    std::string Object::ToString()
    {
        std::stringstream stream;
        stream << "名称：" << name << std::endl;
        stream << "编号：" << instanceID << std::endl;
        stream << "运行中：" << isRunning << std::endl;
        return stream.str();
    }

    void Object::Transfer(Transferrer& transferrer)
    {
        std::string serializationID = ParseSerializationID(this);

        transferrer.TransferString("serializationID", serializationID);
        transferrer.TransferString("name", name);
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
        postAwakeQueue.clear();
    }

    void Object::FlushDestroyQueue()
    {
        for (const auto& object : postDestroyQueue)
        {
            allObjects.erase(object->instanceID);
            delete object;
        }
        postDestroyQueue.clear();
    }
}
