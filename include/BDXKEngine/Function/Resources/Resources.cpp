#include "Resources.h"
#include "BDXKEngine/Platform/Serialization/Serialization.h"

namespace BDXKEngine
{
    ObjectPtrBase Resources::Load(const std::string& path)
    {
        auto serializer = Serialization::CreateBinarySerializer();
        return Serialization::Load("Resources/" + path, serializer);
    }
    void Resources::Save(const std::string& path, const ObjectPtrBase& objectPtr)
    {
        auto serializer = Serialization::CreateBinarySerializer();
        return Serialization::Save("Resources/" + path, objectPtr, serializer);
    }
    bool Resources::IsExisting(const std::string& path)
    {
        return Serialization::IsExisting("Resources/" + path);
    }

    Reflective* Resources::FindSerializationFallback(const Guid& guid)
    {
        if (IsExisting(guid))
            return Serialization::CreateBinarySerializer().Deserialize(ReadFile("Resources/" + guid));
        return nullptr;
    }
    
    void Resources::StaticConstructor()
    {
        ObjectSerializerBase::AddFindSerializationFallback(FindSerializationFallback);
    }
}
