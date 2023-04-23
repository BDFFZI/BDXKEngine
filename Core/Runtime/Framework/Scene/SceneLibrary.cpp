#include "SceneLibrary.h"
#include <filesystem>
#include "Core/Runtime/Base/Object/Serialization/ObjectSerialization.h"

namespace BDXKEngine
{
    std::filesystem::path SceneLibrary::ParsePath(const std::string& fileName)
    {
        return "Library/" + fileName;
    }
    ObjectPtrBase SceneLibrary::Load(const xg::Guid& guid)
    {
        return LoadOfFileSystem(ParsePath(guid.str()));
    }
    void SceneLibrary::Save(const xg::Guid& guid, const ObjectPtrBase& objectPtr)
    {
        SaveOfFileSystem(ParsePath(guid.str()), objectPtr);
    }
    bool SceneLibrary::IsExisting(const xg::Guid& guid)
    {
        return exists(ParsePath(guid.str()));
    }

    ObjectPtrBase SceneLibrary::LoadOfFileSystem(const std::filesystem::path& path)
    {
        if (exists(path) == false)
            throw std::exception("文件不存在");

        // auto serializer = ObjectSerialization::CreateJsonSerializer();
        auto serializer = ObjectSerialization::CreateBinarySerializer();
        ObjectPtrBase result = ObjectSerialization::Load(
            path, serializer,
            xg::Guid{path.filename().string()}, true);
        return result;
    }
    void SceneLibrary::SaveOfFileSystem(const std::filesystem::path& path, const ObjectPtrBase& objectPtr)
    {
        // auto serializer = ObjectSerialization::CreateJsonSerializer();
        auto serializer = ObjectSerialization::CreateBinarySerializer();
        return ObjectSerialization::Save(
            path, objectPtr, serializer,
            xg::Guid{path.filename().string()});
    }

    ObjectPtrBase SceneLibrary::FindSerializationFallback(const xg::Guid& guid)
    {
        const std::filesystem::path path = {ParsePath(guid.str())};
        if (exists(path))
            return LoadOfFileSystem(path);
        return nullptr;
    }

    void SceneLibrary::StaticConstructor()
    {
        if (std::filesystem::exists("Library") == false)
            std::filesystem::create_directory("Library");

        ObjectSerialization::AddDeserializeFallback(FindSerializationFallback);
    }
}
