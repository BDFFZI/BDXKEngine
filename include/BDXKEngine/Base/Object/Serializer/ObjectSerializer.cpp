#include "ObjectSerializer.h"

namespace BDXKEngine
{
    void ObjectSerializerBase::AddFindSerializationFallback(const std::function<Reflective*(const Guid& guid)>& fallback)
    {
        findSerializationFallback.push_back(fallback);
    }
    const std::vector<std::function<Reflective*(const Guid& guid)>>& ObjectSerializerBase::GetFindSerializationFallback()
    {
        return findSerializationFallback;
    }
}
