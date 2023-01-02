#include "ObjectSerializer.h"

namespace BDXKEngine
{
    void ObjectSerializerBase::AddDeserializeFallback(const std::function<ObjectPtrBase(const Guid& guid)>& fallback)
    {
        deserializeFallback.push_back(fallback);
    }
    const std::vector<std::function<ObjectPtrBase(const Guid& guid)>>& ObjectSerializerBase::GetDeserializeFallback()
    {
        return deserializeFallback;
    }
}
