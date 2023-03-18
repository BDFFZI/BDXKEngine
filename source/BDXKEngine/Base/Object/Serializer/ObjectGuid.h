#pragma once
#include <unordered_map>
#include <unordered_set>
#include "BDXKEngine/Base/Data/Guid/Guid.h"
#include "BDXKEngine/Base/Object/Object.h"

namespace BDXKEngine
{
    class ObjectGuid
    {
    public:
        static std::unordered_map<Guid, ID>& GetAllGuids();

        static Guid GetGuid(ID instanceID);
        static Guid GetOrSetGuid(ID instanceID);

        static ID GetInstanceID(const Guid& guid); //当检索到的instance已不存在时也会返回0
        static void SetInstanceID(const Guid& guid, ID instanceID); //当guid未绑定或旧的绑定instance已不存在时可用
        
        static void RemoveGuid(ID instanceID);
        static void RemoveGuid(const Guid& guid);

        static void MarkMainGuid(const Guid& guid);
        static bool IsMainGuid(const Guid& guid);
        static bool IsMainGuid(ID instanceID);
    private:
        static std::unordered_map<ID, Guid> instanceIDToGuid;
        static std::unordered_map<Guid, ID> guidToInstanceID;
        static std::unordered_set<Guid> mainGuid;
    };
}
