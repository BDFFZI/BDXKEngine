#pragma once
#include <string>
#include <unordered_map>

namespace BDXKEngine
{
    class ObjectManager
    {
    public:
        static void FlushDestroyQueue();
        static std::string InstanceIDToGuid(unsigned int instanceID);
        static unsigned int GuidToInstanceID(const std::string& guid);
    };
}
