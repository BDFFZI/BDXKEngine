#include "Resources.h"

#include <filesystem>

#include "BDXKEngine/Platform/Serialization/Serialization.h"

namespace BDXKEngine
{
    ObjectPtrBase Resources::Load(const std::string& path)
    {
        if (IsExisting(path) == false)throw std::exception("文件不存在");

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
        if (std::filesystem::exists("Resources") == false)
            std::filesystem::create_directory("Resources");

        ObjectSerializerBase::AddFindSerializationFallback(FindSerializationFallback);
    }
}
