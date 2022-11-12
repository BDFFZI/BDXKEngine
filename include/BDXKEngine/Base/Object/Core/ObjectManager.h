#pragma once
#include <map>
#include <string>
#include <unordered_map>

namespace BDXKEngine
{
    class ObjectManager
    {
    public:
        static int GetObjectCount();
        
        static std::string InstanceIDToGuid(int instanceID);
        static int GuidToInstanceID(const std::string& guid);

        static std::string SaveObject(int instanceID);
        static int LoadObject(const std::string& guid);
    private:
        inline static std::map<int, std::string> instanceIDToGuid;
        inline static std::map<std::string, int> guidToInstanceID;
    };
}
