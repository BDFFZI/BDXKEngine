#pragma once
#include "BDXKEngine/Base/Object/Serialization/ObjectSerializer.h"
#include "BDXKEngine/Base/Object/Core/Object.h"
#include "BDXKEngine/Base/SerializationExtension/Binary/BinaryExporter2.h"
#include "BDXKEngine/Base/SerializationExtension/Binary/BinaryImporter2.h"
#include "BDXKEngine/Base/SerializationExtension/Json/JsonExporter.h"
#include "BDXKEngine/Base/SerializationExtension/Json/JsonImporter.h"

namespace BDXKEngine
{
    std::string ReadFile(const std::string& path);

    class Resources
    {
    public:
        class JsonSerializerAdapter : public ObjectSerializerAdapter
        {
        protected:
            void TransferSerialization(Transferer& transferer, std::string& serialization) override;
            std::string LoadSerialization(const Guid& guid) override;
        };

        class BinarySerializerAdapter : public ObjectSerializerAdapter
        {
        protected:
            std::string LoadSerialization(const Guid& guid) override;
        };

        static ObjectSerializer<JsonImporter, JsonExporter> CreateJsonSerializer();
        static ObjectSerializer<BinaryImporter2, BinaryExporter2> CreateBinarySerializer();
        static std::string GetRootPath();
        static void SetRootPath(const std::string& path);

        static ObjectPtrBase Load(const std::string& path, Serializer& serializer, bool instantiate = true);
        template <typename TObject>
        static ObjectPtr<TObject> LoadOf(const std::string& path, Serializer& serializer, bool instantiate = true)
        {
            return Load(path, serializer, instantiate).ToObject<TObject>();
        }
        static void Save(const std::string& path, const ObjectPtrBase& objectPtr, Serializer& serializer);

        static ObjectPtrBase Load(const std::string& path);
        template <typename TObject>
        static ObjectPtr<TObject> LoadOf(const std::string& path)
        {
            return Load(path).ToObject<TObject>();
        }
        static void Save(const std::string& path, const ObjectPtrBase& objectPtr);
        static ObjectPtrBase Clone(const ObjectPtrBase& objectPtr, bool instantiate = true);
        template <typename TObject>
        static ObjectPtr<TObject> Clone(const ObjectPtr<TObject>& objectPtr, bool instantiate = true)
        {
            return Clone(static_cast<ObjectPtrBase>(objectPtr), instantiate).template ToObject<TObject>();
        }

        static bool IsResource(const ObjectPtrBase& objectPtr);
        static bool IsExisting(const std::string& path);
    private:
        static std::string rootDirectory;
    };
}
