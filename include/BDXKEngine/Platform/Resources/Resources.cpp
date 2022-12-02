#include "Resources.h"
#include <filesystem>
#include <fstream>

namespace BDXKEngine
{
    const std::string rootDirectory = "Resources/";

    Object* LoadNatice(const Guid& guid, Serializer& serializer)
    {
        std::ifstream ifstream(rootDirectory + guid, std::ios_base::binary);
        const auto size = static_cast<int>(std::filesystem::file_size(rootDirectory + guid));
        const auto buffer = new char[size];
        ifstream.read(buffer, size);
        ifstream.close();
        return dynamic_cast<Object*>(serializer.Deserialize(std::string(buffer, size)));
    }

    class JsonSerializerAdapter : public ObjectSerializerAdapter
    {
        void TransferSerialization(Transferer& transferer, std::string& serialization) override
        {
            dynamic_cast<JsonTransferer&>(transferer).TransferJson("data", serialization);
            if (serialization == "null")serialization = "";
        }

        Object* LoadSerialization(const Guid& guid) override
        {
            ObjectSerializer<JsonImporter, JsonExporter> jsonSerializer = {*this};
            return LoadNatice(guid, jsonSerializer);
        }
    };

    class BinarySerializerAdapter : public ObjectSerializerAdapter
    {
        Object* LoadSerialization(const Guid& guid) override
        {
            ObjectSerializer<BinaryImporter2, BinaryExporter2> binarySerializer = {*this};
            return LoadNatice(guid, binarySerializer);
        }
    };

    JsonSerializerAdapter jsonSerializerAdapter = {};
    BinarySerializerAdapter binarySerializerAdapter = {};

    ObjectSerializer<JsonImporter, JsonExporter> Resources::GetJsonSerializer()
    {
        return ObjectSerializer<JsonImporter, JsonExporter>{jsonSerializerAdapter};
    }
    ObjectSerializer<BinaryImporter2, BinaryExporter2> Resources::GetBinarySerializer()
    {
        return ObjectSerializer<BinaryImporter2, BinaryExporter2>{binarySerializerAdapter};
    }

    ObjectPtrBase Resources::Clone(const ObjectPtrBase& objectPtr, bool instantiate)
    {
        auto binarySerializer = GetBinarySerializer();
        return Object::Clone(objectPtr, binarySerializer, instantiate);
    }

    ObjectPtrBase Resources::Load(const std::string& path, Serializer& serializer, bool instantiate)
    {
        std::ifstream ifstream(rootDirectory + path, std::ios_base::binary);
        const auto size = static_cast<int>(std::filesystem::file_size(rootDirectory + path));
        const auto buffer = new char[size];
        ifstream.read(buffer, size);
        ifstream.close();

        ObjectPtrBase object = dynamic_cast<Object*>(serializer.Deserialize(std::string(buffer, size)));
        if (instantiate)Object::Instantiate(object);
        return object;
    }
    void Resources::Save(const std::string& path, const ObjectPtrBase& objectPtr, Serializer& serializer)
    {
        const std::string data = serializer.Serialize(objectPtr.ToObjectBase());

        //保存文件位置
        // const std::string guid = guidStream[0];
        // guidStream.clear();
        // guidToPath[guid] = path;

        //保存持久化数据
        std::ofstream ofstream(rootDirectory + path, std::ios_base::binary);
        ofstream << data;
        ofstream.close();
    }
    bool Resources::IsResource(const ObjectPtrBase& objectPtr)
    {
        return ObjectSerializerDatabase::IsSerialization(objectPtr.GetInstanceID());
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
}
