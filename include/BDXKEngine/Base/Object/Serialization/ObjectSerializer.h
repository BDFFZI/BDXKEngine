#pragma once
#include "ObjectTransferer.h"
#include "BDXKEngine/Base/Reflection/Reflection.h"
#include "BDXKEngine/Base/Serialization/Core/Serializer.h"
#include "ObjectPtrTransferer.h"
#include "Guid/Guid.h"

namespace BDXKEngine
{
    class ObjectSerializerDatabase
    {
    public:
        static Guid GetOrSetGuid(int instanceID);
        static int GetInstanceID(const Guid& guid);
        static void SetInstanceID(const Guid& guid, int instanceID);

        static void SignRootSerialization(const Guid& guid);
        static bool IsRootSerialization(const Guid& guid);
        static bool IsRootSerialization(int instanceID);
        static bool IsSerialization(int instanceID);
    private:
        static std::unordered_map<int, Guid> instanceIDToGuid;
        static std::unordered_map<Guid, int> guidToInstanceID;
        static std::unordered_set<Guid> rootSerialization;
    };


    template <typename TExporter, typename TImporter>
    class ObjectSerializer : public Serializer
    {
    public:
        ObjectSerializer(std::function<void(Transferer&, std::string&)> transferSerialization): Serializer(
            objectImporter, objectExporter)
        {
            this->transferSerialization = transferSerialization;
        }
        ObjectSerializer(): ObjectSerializer([](Transferer& transferer, std::string& serialization)
        {
            transferer.TransferField("data", serialization);
        })
        {
        }

        std::string Serialize(Reflective* input) override
        {
            if (dynamic_cast<Object*>(input) == nullptr)throw std::exception("只允许序列化Object物体");

            Object* rootObject = dynamic_cast<Object*>(input);
            const int rootInstanceID = rootObject->GetInstanceID();
            const Guid rootGuid = ObjectSerializerDatabase::GetOrSetGuid(rootInstanceID);
            ObjectSerializerDatabase::SignRootSerialization(rootGuid);

            //统计引用
            ObjectPtrTransferer objectPtrTransferer = {rootObject};
            const auto references = objectPtrTransferer.GetReferences();

            //序列化相关物体
            objectExporter.template SetTransferFunc<ObjectPtrBase>([this](const ObjectPtrBase& value)
            {
                const int instanceID = value.GetInstanceID();
                if (Object::FindObjectOfInstanceID(instanceID) == nullptr)
                {
                    Guid guid = {};
                    objectExporter.TransferValue(guid);
                }
                else
                {
                    Guid guid = ObjectSerializerDatabase::GetOrSetGuid(instanceID);
                    objectExporter.TransferValue(guid);
                }
            });
            std::vector<std::tuple<Guid, std::string>> serializations;
            for (auto& instanceID : references)
            {
                Guid guid = ObjectSerializerDatabase::GetOrSetGuid(instanceID);
                if (ObjectSerializerDatabase::IsRootSerialization(instanceID) && instanceID != rootInstanceID)
                    serializations.push_back(std::make_tuple(guid, ""));
                else
                    serializations.push_back(std::make_tuple(guid, Serializer::Serialize(Object::FindObjectOfInstanceID(instanceID))));
            }

            //输出序列化数据
            int serializationsCount = static_cast<int>(serializations.size());
            objectExporter.TransferField("serializationsCount", serializationsCount);
            int index = 0;
            for (auto& [target,data] : serializations)
            {
                std::string itemName = "serialization_" + std::to_string(index);
                objectExporter.PushPath(itemName);
                objectExporter.TransferField("object", target);
                transferSerialization(objectExporter, data);
                objectExporter.PopPath(itemName);

                index++;
            }
            std::string result;
            objectExporter.Reset(result);
            return result;
        }
        Reflective* Deserialize(std::string input) override
        {
            //获取反序列化数据
            objectImporter.Reset(input);
            int serializationsCount;
            objectImporter.TransferField("serializationsCount", serializationsCount);
            std::vector<std::tuple<Guid, std::string>> serializations;
            for (int index = 0; index < serializationsCount; index++)
            {
                Guid guid = {};
                std::string data = {};

                std::string itemName = "serialization_" + std::to_string(index);
                objectImporter.PushPath(itemName);
                objectImporter.TransferField("object", guid);
                transferSerialization(objectImporter, data);
                objectImporter.PopPath(itemName);

                serializations.push_back(std::make_tuple(guid, data));
            }

            const Guid rootGuid = std::get<0>(serializations[0]);
            ObjectSerializerDatabase::SignRootSerialization(rootGuid);

            //反序列化
            std::unordered_map<Guid, std::vector<ObjectPtrBase*>> references;
            objectImporter.template SetTransferFunc<ObjectPtrBase>([&](ObjectPtrBase& value)
            {
                Guid guid;
                objectImporter.TransferValue(guid);

                if (guid.empty())return; //空引用不用管
                if (references.contains(guid) == false)references[guid] = {}; //引用第一次出现

                references[guid].push_back(&value);
            });
            for (const auto& [guid,data] : serializations)
            {
                if (ObjectSerializerDatabase::IsRootSerialization(guid) == false || guid == rootGuid)
                {
                    const Object* object = dynamic_cast<Object*>(Serializer::Deserialize(data));
                    ObjectSerializerDatabase::SetInstanceID(guid, object->GetInstanceID());
                }
            }

            //链接引用关系
            for (const auto& reference : references)
            {
                const ObjectPtrBase object = Object::FindObjectOfInstanceID(ObjectSerializerDatabase::GetInstanceID(reference.first));
                for (const auto objectPtr : reference.second)
                    *objectPtr = object;
            }


            return Object::FindObjectOfInstanceID(ObjectSerializerDatabase::GetInstanceID(rootGuid));
        }
        Reflective* Clone(Reflective* input) override
        {
            const Object* inputObject = dynamic_cast<Object*>(input);
            if (inputObject == nullptr)throw std::exception("只允许序列化Object物体");

            objectExporter.template SetTransferFunc<ObjectPtrBase>([&](const ObjectPtrBase& value)
            {
                int instanceID = value.GetInstanceID();
                objectExporter.TransferValue(instanceID);
            });

            objectImporter.template SetTransferFunc<ObjectPtrBase>([&](ObjectPtrBase& value)
            {
                int instanceID;
                objectImporter.TransferValue(instanceID);
                value = Object::FindObjectOfInstanceID(instanceID);
            });

            return Serializer::Clone(input);
        }
    private:
        ObjectTransferer<TExporter> objectExporter;
        ObjectTransferer<TImporter> objectImporter;
        std::function<void(Transferer&, std::string&)> transferSerialization;
    };
}
