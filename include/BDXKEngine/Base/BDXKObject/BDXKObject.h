#pragma once
#include "BDXKEngine/Base/Object/ObjectSerializer.h"
#include "BDXKEngine/Base/Object/Core/Object.h"
#include "BDXKEngine/Base/SerializationExtension/Binary/BinaryExporter2.h"
#include "BDXKEngine/Base/SerializationExtension/Binary/BinaryImporter2.h"
#include "BDXKEngine/Base/SerializationExtension/Json/JsonExporter.h"
#include "BDXKEngine/Base/SerializationExtension/Json/JsonImporter.h"

namespace BDXKEngine
{
    class BDXKObject
    {
    public:
        static ObjectPtrBase Load(const Guid& guid);
        static Guid Save(const ObjectPtrBase& objectPtr);

        static ObjectPtrBase InstantiateNoAwake(const ObjectPtrBase& objectPtr);
        static ObjectPtrBase Instantiate(const ObjectPtrBase& objectPtr);
        template <typename TObject>
        static ObjectPtr<TObject> InstantiateNoAwake(const ObjectPtr<TObject>& objectPtr)
        {
            return InstantiateNoAwake(static_cast<ObjectPtrBase>(objectPtr)).template ToObject<TObject>();
        }
        template <typename TObject>
        static ObjectPtr<TObject> Instantiate(const ObjectPtr<TObject>& objectPtr)
        {
            return Instantiate(static_cast<ObjectPtrBase>(objectPtr)).template ToObject<TObject>();
        }
    private:
        static const std::string rootDirectory;
        static ObjectSerializer<JsonExporter, JsonImporter> jsonSerializer;
        static ObjectSerializer<BinaryExporter2, BinaryImporter2> binarySerializer;
        
        static std::vector<Guid> guidStream;
        static std::map<Guid, std::string> guidToPath;
    };
}
