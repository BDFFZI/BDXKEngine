#include "ObjectSerializer.h"

namespace BDXKEngine
{
    std::unordered_map<int, Guid> ObjectSerializerDatabase::instanceIDToGuid = {};
    std::unordered_map<Guid, int> ObjectSerializerDatabase::guidToInstanceID = {};
    std::unordered_set<Guid> ObjectSerializerDatabase::serialization = {};

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
        if (guidToInstanceID.count(guid) != 0)
            return guidToInstanceID[guid];
        return 0;
    }
    void ObjectSerializerDatabase::SetInstanceID(const Guid& guid, int instanceID)
    {
        guidToInstanceID[guid] = instanceID;
        instanceIDToGuid[instanceID] = guid;
    }
    void ObjectSerializerDatabase::SignSerialization(const Guid& guid)
    {
        serialization.insert(guid);
    }
    bool ObjectSerializerDatabase::IsSerialization(const Guid& guid)
    {
        return serialization.contains(guid);
    }
    bool ObjectSerializerDatabase::IsSerialization(int instanceID)
    {
        return serialization.contains(GetOrSetGuid(instanceID));
    }
}
