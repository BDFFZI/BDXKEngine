#include "ObjectSerializer.h"

namespace BDXKEngine
{
    std::unordered_map<int, Guid> SerializationDatabase::instanceIDToGuid = {};
    std::unordered_map<Guid, int> SerializationDatabase::guidToInstanceID = {};
    std::unordered_set<Guid> SerializationDatabase::serialization = {};

    Guid SerializationDatabase::GetOrSetGuid(int instanceID)
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
    int SerializationDatabase::GetInstanceID(const Guid& guid)
    {
        if (guidToInstanceID.count(guid) != 0)
            return guidToInstanceID[guid];
        return 0;
    }
    void SerializationDatabase::SetInstanceID(const Guid& guid, int instanceID)
    {
        guidToInstanceID[guid] = instanceID;
        instanceIDToGuid[instanceID] = guid;
    }
    void SerializationDatabase::SignSerialization(const Guid& guid)
    {
        serialization.insert(guid);
    }
    bool SerializationDatabase::IsSerialization(const Guid& guid)
    {
        return serialization.count(guid) != 0;
    }
    bool SerializationDatabase::IsSerialization(int instanceID)
    {
        return serialization.count(GetOrSetGuid(instanceID)) != 0;
    }
}
