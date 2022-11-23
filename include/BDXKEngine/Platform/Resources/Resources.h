#pragma once
#include "BDXKEngine/Base/Object/ObjectSerializer.h"
#include "BDXKEngine/Base/Object/Core/Object.h"
#include "BDXKEngine/Base/SerializationExtension/Binary/BinaryExporter2.h"
#include "BDXKEngine/Base/SerializationExtension/Binary/BinaryImporter2.h"
#include "BDXKEngine/Base/SerializationExtension/Json/JsonExporter.h"
#include "BDXKEngine/Base/SerializationExtension/Json/JsonImporter.h"

namespace BDXKEngine
{
    class Resources
    {
    public:
        static ObjectSerializer<JsonExporter, JsonImporter>& GetJsonSerializer();
        static ObjectSerializer<BinaryExporter2, BinaryImporter2>& GetBinarySerializer();

        static ObjectPtrBase Instantiate(const ObjectPtrBase& objectPtr);
        template <typename TObject>
        static ObjectPtr<TObject> Instantiate(const ObjectPtr<TObject>& objectPtr)
        {
            return Instantiate(static_cast<ObjectPtrBase>(objectPtr)).template ToObject<TObject>();
        }

        static ObjectPtrBase Load(const std::string& path, Serializer& serializer = binarySerializer);
        template <typename TObject>
        static ObjectPtr<TObject> Load(const std::string& path, Serializer& serializer = binarySerializer)
        {
            return Load(path, serializer).ToObject<TObject>();
        }
        static void Save(const std::string& path, const ObjectPtrBase& objectPtr, Serializer& serializer = binarySerializer);
    private:
        static const std::string rootDirectory;
        static ObjectSerializer<JsonExporter, JsonImporter> jsonSerializer;
        static ObjectSerializer<BinaryExporter2, BinaryImporter2> binarySerializer;

        // static std::vector<Guid> guidStream;
        // static std::map<Guid, std::string> guidToPath;
    };
}
