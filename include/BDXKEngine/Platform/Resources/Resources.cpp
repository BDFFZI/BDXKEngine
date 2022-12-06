#include "Resources.h"
#include <filesystem>
#include <fstream>

namespace BDXKEngine
{
    std::string Resources::rootDirectory = "Resources/";

    std::string ReadFile(const std::string& path)
    {
        std::ifstream ifstream(path, std::ios_base::binary);
        if (ifstream.is_open() == false)return {};
        const auto size = static_cast<int>(std::filesystem::file_size(path));
        const std::unique_ptr<char[]> buffer = std::unique_ptr<char[]>(new char[size]);
        ifstream.read(buffer.get(), size);
        ifstream.close();
        return {buffer.get(), static_cast<size_t>(size)};
    }

    std::string Resources::ReadFile(const std::string& path)
    {
        return BDXKEngine::ReadFile(rootDirectory + path);
    }

    std::string Resources::GetRootPath()
    {
        return rootDirectory.substr(0, rootDirectory.size() - 1);
    }
    void Resources::SetRootPath(const std::string& path)
    {
        rootDirectory = path + "/";
    }
    ObjectSerializer<JsonImporter, JsonExporter> Resources::CreateJsonSerializer()
    {
        return ObjectSerializer<JsonImporter, JsonExporter>{
            [](Transferer& transferer, std::string& serialization)
            {
                dynamic_cast<JsonTransferer&>(transferer).TransferJson("data", serialization);
                if (serialization == "null")serialization = "";
            }
        };
    }
    ObjectSerializer<BinaryImporter2, BinaryExporter2> Resources::CreateBinarySerializer()
    {
        return ObjectSerializer<BinaryImporter2, BinaryExporter2>{};
    }

    ObjectPtrBase Resources::Clone(const ObjectPtrBase& objectPtr, bool instantiate)
    {
        auto binarySerializer = CreateBinarySerializer();
        return Object::Clone(objectPtr, binarySerializer, instantiate);
    }
    ObjectPtrBase Resources::Load(const std::string& path, Serializer& serializer, bool instantiate)
    {
        ObjectPtrBase object = dynamic_cast<Object*>(serializer.Deserialize(ReadFile(path)));
        if (instantiate)Object::Instantiate(object);
        return object;
    }
    void Resources::Save(const std::string& path, const ObjectPtrBase& objectPtr, Serializer& serializer)
    {
        const std::string fullPath = rootDirectory + path;
        if (std::filesystem::exists(fullPath.substr(0, fullPath.rfind('/'))) == false)
            throw std::exception("目录不存在");

        const std::string data = serializer.Serialize(objectPtr.ToObjectBase());

        //保存文件位置
        // const std::string guid = guidStream[0];
        // guidStream.clear();
        // guidToPath[guid] = path;

        //保存持久化数据
        std::ofstream ofstream(fullPath, std::ios_base::binary);
        ofstream << data;
        ofstream.close();
    }

    ObjectPtrBase Resources::Load(const std::string& path)
    {
        auto serializer = CreateBinarySerializer();
        return Load(path, serializer);
    }
    void Resources::Save(const std::string& path, const ObjectPtrBase& objectPtr)
    {
        auto serializer = CreateBinarySerializer();
        return Save(path, objectPtr, serializer);
    }

    bool Resources::IsResource(const ObjectPtrBase& objectPtr)
    {
        return ObjectGuid::HasGuid(objectPtr.GetInstanceID());
    }
    bool Resources::IsExisting(const std::string& path)
    {
        std::ifstream ifstream(rootDirectory + path, std::ios_base::binary);
        if (ifstream.is_open())
        {
            ifstream.close();
            return true;
        }
        return false;
    }

    void Resources::StaticConstructor()
    {
        ObjectSerializerBase::AddFindSerializationFallback([](const Guid& guid)
        {
            if (std::filesystem::exists(rootDirectory + guid))
                return ReadFile(guid);
            return std::string{};
        });
    }
}
