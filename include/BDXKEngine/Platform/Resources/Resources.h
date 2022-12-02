#pragma once
#include "BDXKEngine/Base/Object/Serialization/ObjectSerializer.h"
#include "BDXKEngine/Base/Object/Object.h"
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

        static ObjectPtrBase Clone(const ObjectPtrBase& objectPtr, bool instantiate = true);
        template <typename TObject>
        static ObjectPtr<TObject> Clone(const ObjectPtr<TObject>& objectPtr, bool instantiate = true)
        {
            return Clone(static_cast<ObjectPtrBase>(objectPtr), instantiate).template ToObject<TObject>();
        }

        static ObjectPtrBase Load(const std::string& path, Serializer& serializer = binarySerializer, bool instantiate = true);
        template <typename TObject>
        static ObjectPtr<TObject> Load(const std::string& path, Serializer& serializer = binarySerializer, bool instantiate = true)
        {
            return Load(path, serializer, instantiate).ToObject<TObject>();
        }
        static void Save(const std::string& path, const ObjectPtrBase& objectPtr, Serializer& serializer = binarySerializer);
        
        static bool IsResource(const ObjectPtrBase& objectPtr);
        static bool IsExisting(const std::string& path);
    private:
        static const std::string rootDirectory;
        static ObjectSerializer<JsonExporter, JsonImporter> jsonSerializer;
        static ObjectSerializer<BinaryExporter2, BinaryImporter2> binarySerializer;

        // static std::vector<Guid> guidStream;
        // static std::map<Guid, std::string> guidToPath;
    };
}
