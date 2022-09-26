#include "Object.h"
#include "ObjectEvent.h"
#include "Framework/GameObject.h"
#include "Framework/Components/Behavior/Animator.h"
#include "Framework/Components/Renderer/Camera.h"
#include "Framework/Components/Renderer/MeshRenderer.h"

namespace BDXKEngine
{
    int Object::instanceIDCount = 0;
    std::map<int, Object*> Object::allObjects = {};
    std::map<int, Object*> Object::allObjectsInstantiating = {};
    std::vector<Object*> Object::postDestroyQueue;
    std::vector<Object*> Object::postAwakeQueue;

#define TypeSerialize(T)  {String::to_wstring(typeid(T).name()), [] { return Object::Create<T>(); }}
    std::unordered_map<std::wstring, std::function<Object*()>> nameToInstance = {
        TypeSerialize(GameObject),
        TypeSerialize(Component),
        TypeSerialize(Transform),
        TypeSerialize(Animator),
        TypeSerialize(Camera),
        TypeSerialize(Light),
        TypeSerialize(MeshRenderer),
    };

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
        const std::wstring type = importer.TransferString();
        //重新取出数据
        stream.str("");
        serializer->Export(exporter);

        Object* result = nameToInstance[type]();
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
        if (object->isInstantiating == false)
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
        transfer.TransferString({}, String::to_wstring(typeid(*this).name()));
        transfer.TransferInt({}, instanceID);
        transfer.TransferString({}, name);
    }

    void Object::Import(Importer& transfer)
    {
        transfer.TransferString();
        if (transfer.TransferInt() == 0)
            transfer.TransferString();
        else
            name = transfer.TransferString();
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
            allObjectsInstantiating[object->instanceID] = object;
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
            allObjectsInstantiating.erase(object->instanceID);
        }
        for (const auto& object : postDestroyQueue)
        {
            allObjects.erase(object->instanceID);
            delete object;
        }
        postDestroyQueue.clear();
    }
}
