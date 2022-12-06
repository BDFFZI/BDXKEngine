#pragma once
#include "ObjectGuid.h"
#include "BDXKEngine/Base/Object/Core/ObjectPtrTransferer.h"
#include "BDXKEngine/Base/Serialization/Core/Serializer.h"

namespace BDXKEngine
{
    class ObjectSerializerBase : public Serializer
    {
    public:
        static void AddFindSerializationFallback(const std::function<std::string(const Guid& guid)>& fallback);
        static const std::vector<std::function<std::string(const Guid& guid)>>& GetFindSerializationFallback();
    protected:
        ObjectSerializerBase(IOTransferer& importer, IOTransferer& exporter)
            : Serializer(importer, exporter)
        {
        }
    private:
        inline static std::vector<std::function<std::string(const Guid& guid)>> findSerializationFallback = {};
    };

    template <typename TImporter, typename TExporter>
    class ObjectSerializer : public ObjectSerializerBase
    {
    public:
        ObjectSerializer(std::function<void(Transferer&, std::string&)> transferSerialization = [](Transferer& transferer, std::string& serialization)
        {
            transferer.TransferField("data", serialization);
        }):
            ObjectSerializerBase(objectImporter, objectExporter), transferSerialization(transferSerialization)
        {
        }

        std::string Serialize(Reflective* input) override
        {
            if (dynamic_cast<Object*>(input) == nullptr)throw std::exception("只允许序列化Object物体");

            Object* rootObject = dynamic_cast<Object*>(input);
            const int rootInstanceID = rootObject->GetInstanceID();
            const Guid rootGuid = ObjectGuid::GetOrSetGuid(rootInstanceID);
            ObjectGuid::SignMainGuid(rootGuid);

            //统计引用
            ObjectPtrTransferer objectPtrTransferer = {rootObject};
            rootObject->Transfer(objectPtrTransferer);
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
                    Guid guid = ObjectGuid::GetOrSetGuid(instanceID);
                    objectExporter.TransferValue(guid);
                }
            });
            std::vector<std::tuple<Guid, std::string>> serializations;
            for (auto& instanceID : references)
            {
                Guid guid = ObjectGuid::GetOrSetGuid(instanceID);
                if (ObjectGuid::IsMainGuid(instanceID) && instanceID != rootInstanceID)
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
            if (input.empty())throw std::exception("序列化数据为空");

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
            ObjectGuid::SignMainGuid(rootGuid);

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
            for (auto& [guid,data] : serializations)
            {
                //已加载的前置资源
                if (ObjectGuid::IsMainGuid(guid) != false && guid != rootGuid)
                    continue;

                Reflective* reflective = nullptr;
                if (data.empty() == false)
                    reflective = Serializer::Deserialize(data);
                else //未加载的前置资源
                    for (auto& fallback : GetFindSerializationFallback())
                    {
                        data = fallback(guid);
                        if (data.empty() == false)
                        {
                            reflective = ObjectSerializer<TImporter, TExporter>(transferSerialization).Deserialize(data);
                            break;
                        }
                    }

                if (reflective == nullptr)throw std::exception("无法获取序列化数据");
                ObjectGuid::SetInstanceID(guid, dynamic_cast<Object*>(reflective)->GetInstanceID());
            }

            //链接引用关系
            for (const auto& reference : references)
            {
                const ObjectPtrBase object = Object::FindObjectOfInstanceID(ObjectGuid::GetInstanceID(reference.first));
                for (const auto objectPtr : reference.second)
                    *objectPtr = object;
            }


            return Object::FindObjectOfInstanceID(ObjectGuid::GetInstanceID(rootGuid));
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
