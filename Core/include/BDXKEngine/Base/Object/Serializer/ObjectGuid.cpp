#include "ObjectGuid.h"

#include <format>

#include "BDXKEngine/Base/Object/Object.h"

namespace BDXKEngine
{
    std::unordered_map<int, Guid> ObjectGuid::instanceIDToGuid = {};
    std::unordered_map<Guid, int> ObjectGuid::guidToInstanceID = {};
    std::unordered_set<Guid> ObjectGuid::mainGuid = {};


    std::unordered_map<Guid, int>& ObjectGuid::GetAllGuids()
    {
        return guidToInstanceID;
    }
    
    Guid ObjectGuid::GetGuid(int instanceID)
    {
        const auto guidItem = instanceIDToGuid.find(instanceID);
        return guidItem == instanceIDToGuid.end() ? "" : guidItem->second;
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
        if (guidToInstanceID.contains(guid)) //资源重新导入
        {
            const int oldInstanceID = guidToInstanceID[guid];
            if (Object::FindObjectOfInstanceID(oldInstanceID) == nullptr) //Guid已不被使用，允许重新绑定
            {
                guidToInstanceID.erase(guid);
                instanceIDToGuid.erase(oldInstanceID);
            }
            else throw std::exception(std::format("Guid已被绑定至 %i", oldInstanceID).c_str());
        }

        const Guid oldGuid = instanceIDToGuid[instanceID]; //创建物体时可能随机生成默认的Guid，所以允许修改
        if (oldGuid.empty() == false)
        {
            guidToInstanceID.erase(oldGuid);
            instanceIDToGuid.erase(instanceID);
        }

        guidToInstanceID[guid] = instanceID;
        instanceIDToGuid[instanceID] = guid;
    }
    void ObjectGuid::RemoveGuid(int instanceID)
    {
        guidToInstanceID.erase(instanceIDToGuid[instanceID]);
        instanceIDToGuid.erase(instanceID);
    }
    void ObjectGuid::RemoveGuid(const Guid& guid)
    {
        instanceIDToGuid.erase(guidToInstanceID[guid]);
        guidToInstanceID.erase(guid);
    }

    void ObjectGuid::MarkMainGuid(const Guid& guid)
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
