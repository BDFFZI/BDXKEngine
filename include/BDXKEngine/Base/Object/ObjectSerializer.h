#pragma once
#include "ObjectTransferer.h"
#include "BDXKEngine/Base/Reflection/Reflection.h"
#include "BDXKEngine/Base/Serialization/Serializer.h"
#include "Core/ObjectPtrTransferer.h"
#include "Guid/Guid.h"

namespace BDXKEngine
{
    class SerializationDatabase
    {
    public:
        static Guid GetOrSetGuid(int instanceID);
        static int GetInstanceID(const Guid& guid);
        static void SetInstanceID(const Guid& guid, int instanceID);
    private:
        static std::unordered_map<int, Guid> instanceIDToGuid;
        static std::unordered_map<Guid, int> guidToInstanceID;
    };


    template <typename TExporter, typename TImporter>
    class ObjectSerializer : public Serializer
    {
    public:
        ObjectSerializer(std::function<void(Transferer&, const Guid&, std::string&)> transferSerialization): Serializer(
            objectImporter, objectExporter)
        {
            this->transferSerialization = transferSerialization;
        }
        ObjectSerializer(): ObjectSerializer([](Transferer& transferer, const Guid& guid, std::string& serialization)
        {
            transferer.TransferField("data", serialization);
        })
        {
        }


        std::string Serialize(Reflective* input) override
        {
            Object* rootObject = dynamic_cast<Object*>(input);
            if (rootObject == nullptr)throw std::exception("只允许序列化Object物体");

            //统计Object
            ObjectPtrTransferer objectPtrTransferer = {rootObject->GetInstanceID()};
            rootObject->Transfer(objectPtrTransferer);
            const auto references = objectPtrTransferer.GetReferences();

            objectExporter.template SetTransferFunc<ObjectPtrBase>([this](ObjectPtrBase& value)
            {
                const int instanceID = value.GetInstanceID();
                if (Object::FindObjectOfInstanceID(instanceID) == nullptr)
                {
                    Guid guid = {};
                    objectExporter.TransferValue(guid);
                }
                else
                {
                    Guid guid = SerializationDatabase::GetOrSetGuid(instanceID);
                    objectExporter.TransferValue(guid);
                }
            });

            //序列化搜集到的Object
            std::unordered_map<Guid, std::string> serializations;
            for (auto& instanceID : references)
            {
                Guid guid = SerializationDatabase::GetOrSetGuid(instanceID);
                std::string data = Serializer::Serialize(Object::FindObjectOfInstanceID(instanceID));
                serializations[guid] = data;
            }

            //合并结果
            int serializationsCount = static_cast<int>(serializations.size());
            objectExporter.TransferField("serializationsCount", serializationsCount);
            int index = 0;
            for (const auto& item : serializations)
            {
                Guid target = item.first;
                std::string data = item.second;

                std::string itemName = "serialization_" + std::to_string(index);
                objectExporter.PushPath(itemName);
                objectExporter.TransferField("object", target);
                transferSerialization(objectExporter, target, data);
                objectExporter.PopPath(itemName);

                index++;
            }

            std::string result;
            objectExporter.Reset(result);
            return result;
        }

        Reflective* Deserialize(std::string input) override
        {
            std::unordered_map<Guid, std::vector<ObjectPtrBase*>> references;
            objectImporter.template SetTransferFunc<ObjectPtrBase>([&](ObjectPtrBase& value)
            {
                Guid guid;
                objectImporter.TransferValue(guid);

                if (guid.empty())return; //空引用不用管
                if (references.count(guid) == 0)references[guid] = {}; //引用第一次出现

                references[guid].push_back(&value);
            });

            //解析序列化数据
            std::unordered_map<Guid, std::string> serializations;
            objectImporter.Reset(input);
            int serializationsCount;
            objectImporter.TransferField("serializationsCount", serializationsCount);
            for (int index = 0; index < serializationsCount; index++)
            {
                Guid target = {};
                std::string data = {};

                std::string itemName = "serialization_" + std::to_string(index);
                objectImporter.PushPath(itemName);
                objectImporter.TransferField("object", target);
                transferSerialization(objectImporter, target, data);
                objectImporter.PopPath(itemName);

                serializations[target] = data;
            }

            //反序列化解析到的数据
            for (const auto& serialization : serializations)
            {
                Guid target = serialization.first;
                std::string data = serialization.second;
                if (SerializationDatabase::GetInstanceID(target) == 0)
                {
                    Object* object = dynamic_cast<Object*>(Serializer::Deserialize(data));
                    SerializationDatabase::SetInstanceID(target, object->GetInstanceID());
                }
            }

            //链接引用关系
            for (const auto& reference : references)
            {
                ObjectPtrBase object = Object::FindObjectOfInstanceID(SerializationDatabase::GetInstanceID(reference.first));
                for (auto objectPtr : reference.second)
                    *objectPtr = object;
            }

            return Object::FindObjectOfInstanceID(SerializationDatabase::GetInstanceID(serializations.begin()->first));
        }
        Reflective* Clone(Reflective* input) override
        {
            const Object* inputObject = dynamic_cast<Object*>(input);
            if (inputObject == nullptr)throw std::exception("只允许序列化Object物体");

            objectExporter.template SetTransferFunc<ObjectPtrBase>([&](ObjectPtrBase& value)
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
        std::function<void(Transferer&, const Guid&, std::string&)> transferSerialization;
    };
}
