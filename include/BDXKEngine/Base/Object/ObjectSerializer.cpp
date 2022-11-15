#include "ObjectSerializer.h"

namespace BDXKEngine
{
    std::unordered_map<Guid, std::string> GuidDatabase::GetSerializations()
    {
        return guidToSerialization;
    }
    std::unordered_map<Guid, ObjectPtrBase> GuidDatabase::GetObjectPtrBases()
    {
        return guidToObjectPtrBase;
    }
    bool GuidDatabase::GetOrGenerateGuid(int instanceID, Guid& guid)
    {
        const auto guidItem = objectToGuid.find(instanceID);
        if (guidItem == objectToGuid.end())
        {
            guid = NewGuid();
            objectToGuid[instanceID] = guid;
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
    void GuidDatabase::SetObjectPtrBase(const Guid& guid, const ObjectPtrBase& objectPtrBase)
    {
        guidToObjectPtrBase[guid] = objectPtrBase;
    }
}
