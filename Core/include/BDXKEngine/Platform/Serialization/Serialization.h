#pragma once
#include "BDXKEngine/Base/Object/Serializer/ObjectSerializer.h"
#include "BDXKEngine/Base/Object/Object.h"
#include "BDXKEngine/Platform/Serialization/SerializerExtension/Binary/BinaryExporter2.h"
#include "BDXKEngine/Platform/Serialization/SerializerExtension/Binary/BinaryImporter2.h"
#include "BDXKEngine/Platform/Serialization/SerializerExtension/Json/JsonExporter.h"
#include "BDXKEngine/Platform/Serialization/SerializerExtension/Json/JsonImporter.h"

namespace BDXKEngine
{
    class Serialization
    {
    public:
        static ObjectSerializer<JsonImporter, JsonExporter> CreateJsonSerializer();
        static ObjectSerializer<BinaryImporter2, BinaryExporter2> CreateBinarySerializer();

        static ObjectPtrBase Load(const std::string& path, ObjectSerializerBase& serializer, bool instantiate = true, bool persistent = true);
        template <typename TObject>
        static ObjectPtr<TObject> Load(const std::string& path, ObjectSerializerBase& serializer, bool instantiate = true, bool persistent = true)
        {
            return Load(path, serializer, instantiate, persistent).ToObject<TObject>();
        }
        static void Save(const std::string& path, const ObjectPtrBase& objectPtr, ObjectSerializerBase& serializer);
        static ObjectPtrBase Clone(const ObjectPtrBase& objectPtr, bool instantiate = true);
        template <typename TObject>
        static ObjectPtr<TObject> Clone(const ObjectPtr<TObject>& objectPtr, bool instantiate = true)
        {
            return Clone(static_cast<ObjectPtrBase>(objectPtr), instantiate).template ToObject<TObject>();
        }

        static bool IsPersistent(const ObjectPtrBase& objectPtr);
        static bool IsExisting(const std::string& path);

        static void MarkPersistent(int instanceID);
        static void UnMarkPersistent(int instanceID);
    };
}
