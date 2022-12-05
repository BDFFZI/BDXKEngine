#pragma once
#include <unordered_map>
#include <unordered_set>
#include "BDXKEngine/Base/Object/Guid/Guid.h"

namespace BDXKEngine
{
    class ObjectGuid
    {
    public:
        static bool HasGuid(int instanceID);
        static Guid GetGuid(int instanceID);
        static int GetInstanceID(const Guid& guid);
        static Guid GetOrSetGuid(int instanceID);
        static void SetInstanceID(const Guid& guid, int instanceID);

        static void SignMainGuid(const Guid& guid);
        static bool IsMainGuid(const Guid& guid);
        static bool IsMainGuid(int instanceID);
    private:
        static std::unordered_map<int, Guid> instanceIDToGuid;
        static std::unordered_map<Guid, int> guidToInstanceID;
        static std::unordered_set<Guid> mainGuid;
    };
}
