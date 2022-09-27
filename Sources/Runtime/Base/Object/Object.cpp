#include "Object.h"
#include "ObjectEvent.h"

namespace BDXKEngine
{
    int Object::instanceIDCount = 0;
    std::map<int, Object*> Object::allObjects = {};
    std::map<int, Object*> Object::allObjectsRunning = {};
    std::vector<Object*> Object::postDestroyQueue;
    std::vector<Object*> Object::postAwakeQueue;

    std::unordered_map<std::wstring, std::function<Object*()>> Object::serializationID = {};

    void Object::DebugObjectCount()
    {
        Debug::LogWarning(L"当前所有Object数量:" + std::to_wstring(allObjects.size()));
    }

    Object* Object::InstantiateNoAwake(Object* serializer)
    {
        //取出类型信息
        std::stringstream stream = {};
        BinaryExporter exporter = {stream};
        serializer->Export(exporter);
        BinaryImporter importer = {stream};
        const std::wstring type = importer.TransferString({});
        //重新取出数据
        stream.str("");
        serializer->Export(exporter);

        Object* result = serializationID[type]();
        result->Import(importer);
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

    std::wstring Object::GetName()
    {
        return name;
    }

    void Object::SetName(const std::wstring& name)
    {
        this->name = name;
    }

    bool Object::GetIsRunning() const
    {
        return isRunning;
    }

    std::wstring Object::ToString()
    {
        std::wstringstream stream;
        stream << L"名称：" << name << std::endl;
        stream << L"编号：" << instanceID << std::endl;
        stream << L"运行中：" << isRunning << std::endl;
        return stream.str();
    }

    void Object::Export(Exporter& transfer)
    {
        transfer.TransferString({}, String::to_wstring(typeid(*this).name()));
        transfer.TransferInt({}, instanceID);
        transfer.TransferString({}, name);
    }

    void Object::Import(Importer& transfer)
    {
        transfer.TransferString({});
        transfer.TransferInt({});
        const std::wstring sourceName = transfer.TransferString({});
        if (sourceName.empty() == false)
            name = sourceName;
    }

    void Object::Awake()
    {
        //Debug::LogWarning(static_cast<String>(L"Object::Awake ") + instanceID + " " + name);
    }

    void Object::Destroy()
    {
        //Debug::LogWarning(static_cast<String>(L"Object::Destroy ") + instanceID + " " + name);
    }

    void Object::FlushAwakeQueue()
    {
        for (const auto& object : postAwakeQueue)
        {
            allObjectsRunning[object->instanceID] = object;
            if (const auto handler = dynamic_cast<AwakeHandler*>(object); handler != nullptr)
                handler->OnAwake();
        }
        postAwakeQueue.clear();
    }

    void Object::FlushDestroyQueue()
    {
        for (const auto& object : postDestroyQueue)
        {
            if (const auto handler = dynamic_cast<DestroyHandler*>(object); handler != nullptr)
                handler->OnDestroy();
            allObjectsRunning.erase(object->instanceID);
        }
        for (const auto& object : postDestroyQueue)
        {
            allObjects.erase(object->instanceID);
            delete object;
        }
        postDestroyQueue.clear();
    }
}
