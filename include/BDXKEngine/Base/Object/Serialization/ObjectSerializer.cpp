#include "ObjectSerializer.h"

namespace BDXKEngine
{
    std::unordered_map<int, Guid> ObjectSerializerDatabase::instanceIDToGuid = {};
    std::unordered_map<Guid, int> ObjectSerializerDatabase::guidToInstanceID = {};
    std::unordered_set<Guid> ObjectSerializerDatabase::rootSerialization = {};

    Guid ObjectSerializerDatabase::GetOrSetGuid(int instanceID)
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
    int ObjectSerializerDatabase::GetInstanceID(const Guid& guid)
    {
        if (guidToInstanceID.contains(guid))
            return guidToInstanceID[guid];
        return 0;
    }
    void ObjectSerializerDatabase::SetInstanceID(const Guid& guid, int instanceID)
    {
        guidToInstanceID[guid] = instanceID;
        instanceIDToGuid[instanceID] = guid;
    }
    void ObjectSerializerDatabase::SignRootSerialization(const Guid& guid)
    {
        rootSerialization.insert(guid);
    }
    bool ObjectSerializerDatabase::IsRootSerialization(const Guid& guid)
    {
        return rootSerialization.contains(guid);
    }
    bool ObjectSerializerDatabase::IsRootSerialization(int instanceID)
    {
        return rootSerialization.contains(GetOrSetGuid(instanceID));
    }
    bool ObjectSerializerDatabase::IsSerialization(int instanceID)
    {
        return instanceIDToGuid.contains(instanceID);
    }
}
