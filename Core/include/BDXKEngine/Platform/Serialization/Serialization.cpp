#include "Serialization.h"
#include <filesystem>
#include <fstream>

namespace BDXKEngine
{
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

    ObjectSerializer<JsonImporter, JsonExporter> Serialization::CreateJsonSerializer()
    {
        return ObjectSerializer<JsonImporter, JsonExporter>{
            [](Transferer& transferer, std::string& serialization)
            {
                dynamic_cast<JsonTransferer&>(transferer).TransferJson("data", serialization);
                if (serialization == "null")serialization = "";
            }
        };
    }
    ObjectSerializer<BinaryImporter2, BinaryExporter2> Serialization::CreateBinarySerializer()
    {
        return ObjectSerializer<BinaryImporter2, BinaryExporter2>{};
    }
    ObjectPtrBase Serialization::Load(const std::string& path, ObjectSerializerBase& serializer, bool instantiate, bool persistent)
    {
        std::vector<Guid> guids = {};
        ObjectPtrBase object = serializer.Deserialize(ReadFile(path), &guids);

        if (persistent == false)
            for (auto& guid : guids)
                UnMarkPersistent(ObjectGuid::GetInstanceID(guid));
        if (instantiate)Object::Instantiate(object);

        return object;
    }
    void Serialization::Save(const std::string& path, const ObjectPtrBase& objectPtr, ObjectSerializerBase& serializer)
    {
        if (std::filesystem::exists(path.substr(0, path.rfind('/'))) == false)
            throw std::exception("目录不存在");

        const std::string data = serializer.Serialize(objectPtr);

        //保存文件位置
        // const std::string guid = guidStream[0];
        // guidStream.clear();
        // guidToPath[guid] = path;

        //保存持久化数据
        std::ofstream ofstream(path, std::ios_base::binary);
        ofstream << data;
        ofstream.close();
    }
    ObjectPtrBase Serialization::Clone(const ObjectPtrBase& objectPtr, bool instantiate)
    {
        auto binarySerializer = CreateBinarySerializer();

        if (objectPtr.IsNull()) throw std::exception("实例化的物体为空");

        //克隆并注册
        ObjectPtrBase instance = binarySerializer.Clone(objectPtr.ToObjectBase());
        if (instantiate)Object::Instantiate(instance);
        return instance;
    }


    bool Serialization::IsPersistent(const ObjectPtrBase& objectPtr)
    {
        return ObjectGuid::HasGuid(objectPtr.GetInstanceID());
    }
    bool Serialization::IsExisting(const std::string& path)
    {
        std::ifstream ifstream(path, std::ios_base::binary);
        if (ifstream.is_open())
        {
            ifstream.close();
            return true;
        }
        return false;
    }
    void Serialization::MarkPersistent(int instanceID)
    {
        ObjectGuid::GetOrSetGuid(instanceID);
    }
    void Serialization::UnMarkPersistent(int instanceID)
    {
        ObjectGuid::RemoveGuid(instanceID);
    }
}
