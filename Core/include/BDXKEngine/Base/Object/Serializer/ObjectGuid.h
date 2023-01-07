#pragma once
#include <unordered_map>
#include <unordered_set>
#include "BDXKEngine/Base/Data/Guid/Guid.h"

namespace BDXKEngine
{
    class ObjectGuid
    {
    public:
        static std::unordered_map<Guid, int>& GetAllGuids();

        static Guid GetGuid(int instanceID);
        static Guid GetOrSetGuid(int instanceID);

        static int GetInstanceID(const Guid& guid); //当检索到的instance已不存在时也会返回0
        static void SetInstanceID(const Guid& guid, int instanceID); //当guid未绑定或旧的绑定instance已不存在时可用
        
        static void RemoveGuid(int instanceID);
        static void RemoveGuid(const Guid& guid);

        static void MarkMainGuid(const Guid& guid);
        static bool IsMainGuid(const Guid& guid);
        static bool IsMainGuid(int instanceID);
    private:
        static std::unordered_map<int, Guid> instanceIDToGuid;
        static std::unordered_map<Guid, int> guidToInstanceID;
        static std::unordered_set<Guid> mainGuid;
    };
}
