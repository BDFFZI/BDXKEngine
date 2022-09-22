#pragma once
#include <string>

namespace BDXKEngine
{
    class ObjectManager
    {
    protected:
        static void FlushDestroyBuffer();
        static std::wstring InstanceIDToGuid(unsigned int instanceID);
        static unsigned int GuidToInstanceID(const std::wstring& guid);
    };
}
