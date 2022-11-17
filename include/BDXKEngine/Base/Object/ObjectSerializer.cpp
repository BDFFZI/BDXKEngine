#include "ObjectSerializer.h"

namespace BDXKEngine
{
    std::unordered_map<int, Guid> SerializationDatabase::instanceIDToGuid = {};
    std::unordered_map<Guid, int> SerializationDatabase::guidToInstanceID = {};

    Guid SerializationDatabase::GetOrSetGuid(int instanceID)
    {
        const auto guidItem = instanceIDToGuid.find(instanceID);
        if (guidItem == instanceIDToGuid.end())
        {
            Guid guid = NewGuid();
            instanceIDToGuid[instanceID] = guid;
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
}
