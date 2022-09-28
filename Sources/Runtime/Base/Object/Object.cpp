#include "Object.h"

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
        serializer->Transfer(exporter);
        BinaryImporter importer = {stream};
        std::wstring typenameTemp;
        importer.TransferString(L"typename", typenameTemp);
        //重新取出数据
        stream.str("");
        serializer->Transfer(exporter);
        //创建实例
        Object* result = serializationID[typenameTemp]();
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

    void Object::Transfer(Transferrer& transferrer)
    {
        std::wstring typenameTemp = String::to_wstring(typeid(*this).name());

        transferrer.TransferString(L"typename", typenameTemp);
        transferrer.TransferString(L"name", name);
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
