#include "ObjectGuid.h"

#include "BDXKEngine/Base/Object/Core/Object.h"

namespace BDXKEngine
{
    std::unordered_map<int, Guid> ObjectGuid::instanceIDToGuid = {};
    std::unordered_map<Guid, int> ObjectGuid::guidToInstanceID = {};
    std::unordered_set<Guid> ObjectGuid::mainGuid = {};

    bool ObjectGuid::HasGuid(int instanceID)
    {
        return instanceIDToGuid.contains(instanceID);
    }
    Guid ObjectGuid::GetGuid(int instanceID)
    {
        if (instanceIDToGuid.contains(instanceID) == false)
            throw std::exception("目标物体没有Guid");
        return instanceIDToGuid[instanceID];
    }
    Guid ObjectGuid::GetOrSetGuid(int instanceID)
    {
        const auto guidItem = instanceIDToGuid.find(instanceID);
        if (guidItem == instanceIDToGuid.end())
        {
            Guid guid = NewGuid();
            SetInstanceID(guid, instanceID);
            return guid;
        }
        else
        {
            Guid guid = guidItem->second;
            return guid;
        }
    }
    int ObjectGuid::GetInstanceID(const Guid& guid)
    {
        if (guidToInstanceID.contains(guid))
        {
            const int instanceID = guidToInstanceID[guid];
            if (Object::FindObjectOfInstanceID(instanceID) == nullptr)
                return 0;
            return instanceID;
        }
        return 0;
    }
    void ObjectGuid::SetInstanceID(const Guid& guid, int instanceID)
    {
        guidToInstanceID[guid] = instanceID;
        instanceIDToGuid[instanceID] = guid;
    }

    void ObjectGuid::SignMainGuid(const Guid& guid)
    {
        mainGuid.insert(guid);
    }
    bool ObjectGuid::IsMainGuid(const Guid& guid)
    {
        return mainGuid.contains(guid);
    }
    bool ObjectGuid::IsMainGuid(int instanceID)
    {
        return mainGuid.contains(GetOrSetGuid(instanceID));
    }
}
