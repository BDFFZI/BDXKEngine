#include "ObjectManager.h"
#include <objbase.h>
#include "Object.h"

namespace BDXKEngine
{
    int ObjectManager::GetObjectCount()
    {
        return static_cast<int>(Object::allObjects.size());
    }

    std::string ObjectManager::InstanceIDToGuid(int instanceID)
    {
        const auto guidItem = instanceIDToGuid.find(instanceID);
        return guidItem == instanceIDToGuid.end()
                   ? ""
                   : guidItem->second;
    }

    int ObjectManager::GuidToInstanceID(const std::string& guid)
    {
        const auto instanceIDItem = guidToInstanceID.find(guid);
        return instanceIDItem == guidToInstanceID.end()
                   ? 0
                   : instanceIDItem->second;
    }
    std::string ObjectManager::SaveObject(int instanceID)
    {
        std::string guid = InstanceIDToGuid(instanceID);
        if (guid.empty())
        {
            GUID nativeGuid = {};
            CoCreateGuid(&nativeGuid);

            char data[64];
            const int size = sprintf_s(
                data, "{%08X-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X}",
                nativeGuid.Data1, nativeGuid.Data2, nativeGuid.Data3,
                nativeGuid.Data4[1], nativeGuid.Data4[0], nativeGuid.Data4[7], nativeGuid.Data4[6],
                nativeGuid.Data4[5], nativeGuid.Data4[4], nativeGuid.Data4[3], nativeGuid.Data4[2]
            );

            guid = std::string(data);
            guid.resize(size);

            instanceIDToGuid[instanceID] = guid;
            guidToInstanceID[guid] = instanceID;
        }

        //TODO 保存持久化数据

        return guid;
    }
    int ObjectManager::LoadObject(const std::string& guid)
    {
        //TODO 读取持久化数据

        return 0;
    }
}
