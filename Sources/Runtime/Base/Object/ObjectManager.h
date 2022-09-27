#pragma once
#include <string>

namespace BDXKEngine
{
    class ObjectManager
    {
    protected:
        static void FlushDestroyQueue();
        static std::wstring InstanceIDToGuid(unsigned int instanceID);
        static unsigned int GuidToInstanceID(const std::wstring& guid);
    };
}
