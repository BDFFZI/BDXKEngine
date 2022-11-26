#include "Resources.h"
#include <filesystem>
#include <fstream>

namespace BDXKEngine
{
    // std::vector<Guid> BDXKObject::guidStream = {};
    // std::map<Guid, std::string> BDXKObject::guidToPath = {};

    const std::string Resources::rootDirectory = "Resources/";
    ObjectSerializer<JsonExporter, JsonImporter> Resources::jsonSerializer = {
        [](Transferer& transferer, std::string& serialization)
        {
            dynamic_cast<JsonTransferer&>(transferer).TransferJson("data", serialization);
        }
    };
    ObjectSerializer<BinaryExporter2, BinaryImporter2> Resources::binarySerializer;
    
    ObjectSerializer<JsonExporter, JsonImporter>& Resources::GetJsonSerializer()
    {
        return jsonSerializer;
    }
    ObjectSerializer<BinaryExporter2, BinaryImporter2>& Resources::GetBinarySerializer()
    {
        return binarySerializer;
    }

    ObjectPtrBase Resources::Clone(const ObjectPtrBase& objectPtr, bool instantiate)
    {
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
}
