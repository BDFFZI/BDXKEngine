#include "Object.h"
#include "ObjectEvent.h"

namespace BDXKEngine
{
    unsigned int Object::instanceIDCount = 0;
    std::map<unsigned int, Object*> Object::allObjects = {};
    std::map<unsigned int, Object*> Object::allObjectsRuntime = {};
    std::vector<Object*> Object::postDestroyQueue;
    std::vector<Object*> Object::postAwakeQueue;

    void Object::DebugObjectCount()
    {
        Debug::LogWarning(L"当前所有Object数量:" + std::to_wstring(allObjects.size()));
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
        if (object == nullptr || object->isInstantiating)
            return;

        object->isInstantiating = true;
        object->Awake();
        postAwakeQueue.push_back(object);
    }

    void Object::Destroy(Object* object)
    {
        if (object == nullptr || object->isDestroying)
            return;
        object->isDestroying = true;
        object->Destroy();
        postDestroyQueue.push_back(object);
    }


    Object* Object::FindObjectOfInstanceID(const unsigned int instanceID)
    {
        if (const auto pair = allObjects.find(instanceID); pair != allObjects.end())
            return pair->second;

        return nullptr;
    }

    unsigned int Object::GetInstanceID() const
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

    bool Object::GetIsInstantiating() const
    {
        return isInstantiating;
    }

    std::wstring Object::ToString()
    {
        return name + L"\n" + std::to_wstring(instanceID);
    }

    void Object::Export(Exporter& transfer)
    {
        transfer.TransferBytes(&instanceID, sizeof(instanceID));
        transfer.TransferString(name);
    }

    void Object::Import(Importer& transfer)
    {
        unsigned int instanceID = 0;
        if (transfer.TransferBytes(&instanceID, sizeof(instanceID)); instanceID == 0)
            transfer.TransferString();
        else
            name = transfer.TransferString();
    }

    void Object::Awake()
    {
        Debug::LogWarning(static_cast<String>(L"Object::Awake ") + instanceID + " " + name);
    }

    void Object::Destroy()
    {
        Debug::LogWarning(static_cast<String>(L"Object::Destroy ") + instanceID + " " + name);
    }

    void Object::FlushAwakeQueue()
    {
        for (const auto& object : postAwakeQueue)
        {
            allObjectsRuntime[object->instanceID] = object;
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
            allObjectsRuntime.erase(object->instanceID);
        }
        for (const auto& object : postDestroyQueue)
        {
            allObjects.erase(object->instanceID);
            delete object;
        }
        postDestroyQueue.clear();
    }
}
