#include "Serialization.h"
#include <filesystem>
#include <fstream>

namespace BDXKEngine
{
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

        //保存持久化数据
        const std::string data = serializer.Serialize(objectPtr);
        WriteFile(path, data);
    }
    ObjectPtrBase Serialization::Clone(const ObjectPtrBase& objectPtr, bool instantiate)
    {
        if (objectPtr.IsNull()) throw std::exception("实例化的物体为空");

        //克隆
        auto binarySerializer = CreateBinarySerializer();
        ObjectPtrBase instance = binarySerializer.Clone(objectPtr.ToObjectBase());

        if (instantiate)Object::Instantiate(instance);
        return instance;
    }


    bool Serialization::IsPersistent(const ObjectPtrBase& objectPtr)
    {
        return ObjectGuid::GetGuid(objectPtr.GetInstanceID()).empty() == false;
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
