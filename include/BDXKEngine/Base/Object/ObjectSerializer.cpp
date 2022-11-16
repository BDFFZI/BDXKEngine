#include "ObjectSerializer.h"

namespace BDXKEngine
{
    std::unordered_map<Guid, std::string> GuidDatabase::GetSerializations()
    {
        return guidToSerialization;
    }
    std::unordered_map<Guid, int> GuidDatabase::GetInstanceIDs()
    {
        return guidToInstanceID;
    }
    bool GuidDatabase::GetOrGenerateGuid(int instanceID, Guid& guid)
    {
        const auto guidItem = instanceIDToGuid.find(instanceID);
        if (guidItem == instanceIDToGuid.end())
        {
            guid = NewGuid();
            instanceIDToGuid[instanceID] = guid;
            return false;
        }
        else
        {
            guid = guidItem->second;
            return true;
        }
    }
    void GuidDatabase::SetSerialization(const Guid& guid, const std::string& serialization)
    {
        guidToSerialization[guid] = serialization;
    }
    void GuidDatabase::SetInstanceID(const Guid& guid, int instanceID)
    {
        guidToInstanceID[guid] = instanceID;
    }
}
