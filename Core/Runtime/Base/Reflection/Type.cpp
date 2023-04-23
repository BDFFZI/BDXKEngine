#include "Type.h"

namespace BDXKEngine
{
    std::string GetTypeName(const Type& type)
    {
        return type.substr(type.rfind(':') + 1);
    }
}
