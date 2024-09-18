#pragma once
#include "Core/Runtime/Base/Object/Serialization/ObjectSerializer.h"
#include "Core/Runtime/Base/Serializer/Binary/BinaryReader.h"
#include "Core/Runtime/Base/Serializer/Binary/BinaryWriter.h"
#include "Core/Runtime/Base/Serializer/Json/JsonReader.h"
#include "Core/Runtime/Base/Serializer/Json/JsonWriter.h"

namespace BDXKEngine
{
    class ObjectSerialization
    {
    public:
        static void AddDeserializeFallback(const std::function<ObjectPtrBase(const xg::Guid& guid)>& fallback);

        static ObjectSerializer<JsonReader, JsonWriter> CreateJsonSerializer();
        static ObjectSerializer<BinaryReader, BinaryWriter> CreateBinarySerializer();

        static ObjectPtrBase Load(const std::filesystem::path& path, ObjectSerializerBase& serializer,
                                  const xg::Guid& persistent, bool instantiate);
        template <typename TObject>
        static ObjectPtr<TObject> Load(const std::filesystem::path& path, ObjectSerializerBase& serializer,
                                       const xg::Guid& persistent, const bool instantiate)
        {
            return Load(path, serializer, persistent, instantiate).template ToObject<TObject>();
        }

        static void Save(const std::filesystem::path& path, const ObjectPtrBase& objectPtr, ObjectSerializerBase& serializer,
                         const xg::Guid& persistent);

        static ObjectPtrBase Clone(const ObjectPtrBase& objectPtr, bool instantiate = true);
        template <typename TObject>
        static ObjectPtr<TObject> Clone(const ObjectPtr<TObject>& objectPtr, const bool instantiate = true)
        {
            return Clone(static_cast<ObjectPtrBase>(objectPtr), instantiate).template ToObject<TObject>();
        }

        static bool IsPersistent(const ObjectPtrBase& object);
        static xg::Guid GetPersistent(const ObjectPtrBase& object);
        static void MarkPersistent(const ObjectPtrBase& object, xg::Guid guid);
        static void UnMarkPersistent(const ObjectPtrBase& object);

    private:
        inline static std::vector<std::function<ObjectPtrBase(const xg::Guid& guid)>> deserializeFallbacks = {};
        static ObjectPtrBase DeserializeFallback(const ObjectRef& other);
    };
}
