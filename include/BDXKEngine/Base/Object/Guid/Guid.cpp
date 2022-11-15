#include "Guid.h"
#include <objbase.h>

namespace BDXKEngine
{
    Guid NewGuid()
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

        std::string guid = std::string(data);
        guid.resize(size);

        return guid;
    }
}
