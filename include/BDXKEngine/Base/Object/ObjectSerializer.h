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

        static void SignSerialization(const Guid& guid);
        static bool IsSerialization(const Guid& guid);
        static bool IsSerialization(int instanceID);
    private:
        static std::unordered_map<int, Guid> instanceIDToGuid;
        static std::unordered_map<Guid, int> guidToInstanceID;
        static std::unordered_set<Guid> serialization;
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
            const Guid rootGuid = SerializationDatabase::GetOrSetGuid(rootInstanceID);
            SerializationDatabase::SignSerialization(rootGuid);

            //统计引用
            ObjectPtrTransferer objectPtrTransferer = {rootObject->GetInstanceID()};
            rootObject->Transfer(objectPtrTransferer);
            const auto references = objectPtrTransferer.GetReferences();

            //序列化相关物体
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
            std::unordered_map<Guid, std::string> serializations;
            for (auto& instanceID : references)
            {
                Guid guid = SerializationDatabase::GetOrSetGuid(instanceID);
                if (SerializationDatabase::IsSerialization(instanceID) && instanceID != rootInstanceID)
                    serializations[guid] = "";
                else
                    serializations[guid] = Serializer::Serialize(Object::FindObjectOfInstanceID(instanceID));
            }

            //合并序列化数据
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
            std::unordered_map<Guid, std::vector<ObjectPtrBase*>> references;
            objectImporter.template SetTransferFunc<ObjectPtrBase>([&](ObjectPtrBase& value)
            {
                Guid guid;
                objectImporter.TransferValue(guid);

                if (guid.empty())return; //空引用不用管
                if (references.count(guid) == 0)references[guid] = {}; //引用第一次出现
                
                references[guid].push_back(&value);
            });

            //分离序列化数据
            objectImporter.Reset(input);
            int serializationsCount;
            objectImporter.TransferField("serializationsCount", serializationsCount);
            std::unordered_map<Guid, std::string> serializations;
            for (int index = 0; index < serializationsCount; index++)
            {
                Guid target = {};
                std::string data = {};

                std::string itemName = "serialization_" + std::to_string(index);
                objectImporter.PushPath(itemName);
                objectImporter.TransferField("object", target);
                transferSerialization(objectImporter, data);
                objectImporter.PopPath(itemName);

                serializations[target] = data;
            }

            Guid rootGuid = serializations.begin()->first;
            SerializationDatabase::SignSerialization(rootGuid);

            //反序列化相关物体
            for (const auto& serialization : serializations)
            {
                Guid target = serialization.first;
                std::string data = serialization.second;

                //TODO 重新加载
                if (SerializationDatabase::IsSerialization(target) == false || target == rootGuid)
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
        std::function<void(Transferer&, std::string&)> transferSerialization;
    };
}
