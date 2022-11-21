#include "Resource.h"
#include <filesystem>
#include <fstream>

namespace BDXKEngine
{
    // std::vector<Guid> BDXKObject::guidStream = {};
    // std::map<Guid, std::string> BDXKObject::guidToPath = {};

    const std::string Resource::rootDirectory = "C:/Users/BDFFZI/Desktop/BDXKTemp/";
    ObjectSerializer<JsonExporter, JsonImporter> Resource::jsonSerializer = {
        [](Transferer& transferer, std::string& serialization)
        {
            dynamic_cast<JsonTransferer&>(transferer).TransferJson("data", serialization);
        }
    };
    ObjectSerializer<BinaryExporter2, BinaryImporter2> Resource::binarySerializer;

    ObjectPtrBase Resource::Load(const std::string& path)
    {
        std::ifstream ifstream(path, std::ios_base::binary);
        const auto size = static_cast<int>(std::filesystem::file_size(path));
        const auto buffer = new char[size];
        ifstream.read(buffer, size);
        ifstream.close();

        const Object* object = dynamic_cast<Object*>(jsonSerializer.Deserialize(std::string(buffer, size)));
        return Object::Instantiate(object);
    }
    void Resource::Save(const std::string& path, const ObjectPtrBase& objectPtr)
    {
        const std::string data = jsonSerializer.Serialize(objectPtr.ToObjectBase());

        //保存文件位置
        // const std::string guid = guidStream[0];
        // guidStream.clear();
        // guidToPath[guid] = path;

        //保存持久化数据
        std::ofstream ofstream(path, std::ios_base::binary);
        ofstream << data;
        ofstream.close();
    }
    ObjectPtrBase Resource::Instantiate(const ObjectPtrBase& objectPtr)
    {
        return Object::Instantiate(objectPtr, binarySerializer);
    }
}
