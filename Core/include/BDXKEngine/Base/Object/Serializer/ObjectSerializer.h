#pragma once
#include "ObjectGuid.h"
#include "BDXKEngine/Base/Object/Object.h"
#include "BDXKEngine/Base/Object/Transferer/ObjectPtrTransferer.h"
#include "BDXKEngine/Base/Serializer/Core/Serializer.h"

namespace BDXKEngine
{
    class ObjectSerializerBase
    {
    public:
        static void AddDeserializeFallback(const std::function<ObjectPtrBase(const Guid& guid)>& fallback);
        static const std::vector<std::function<ObjectPtrBase(const Guid& guid)>>& GetDeserializeFallback();

        virtual ~ObjectSerializerBase() = default;
        virtual std::string Serialize(ObjectPtrBase input) = 0;
        virtual ObjectPtrBase Deserialize(std::string input) = 0;
        virtual ObjectPtrBase Clone(ObjectPtrBase input) = 0;
    private:
        inline static std::vector<std::function<ObjectPtrBase(const Guid& guid)>> deserializeFallback = {};
    };

    template <typename TImporter, typename TExporter>
    class ObjectSerializer : public ObjectSerializerBase
    {
    public:
        ObjectSerializer(std::function<void(Transferer&, std::string&)> transferSerialization = [](Transferer& transferer, std::string& serialization)
        {
            transferer.TransferField("data", serialization);
        }):
            serializer(objectImporter, objectExporter), transferSerialization(transferSerialization)
        {
        }

        std::string Serialize(ObjectPtrBase input) override
        {
            Object* rootObject = input.ToObjectBase();
            if (rootObject == nullptr)
                throw std::exception("目标不存在");

            //获取目标Guid信息
            const int rootInstanceID = rootObject->GetInstanceID();
            const Guid rootGuid = ObjectGuid::GetOrSetGuid(rootInstanceID);
            ObjectGuid::SignMainGuid(rootGuid);

            //统计需要序列化的物体
            ObjectPtrTransferer objectPtrTransferer = {rootObject};
            rootObject->Transfer(objectPtrTransferer);
            const auto references = objectPtrTransferer.GetReferences();

            //设置序列化物体的方式（将物体引用转为Guid）
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

            //序列化
            std::vector<std::tuple<Guid, std::string>> serializations;
            for (auto& instanceID : references)
            {
                Guid guid = ObjectGuid::GetOrSetGuid(instanceID);
                if (ObjectGuid::IsMainGuid(instanceID) && instanceID != rootInstanceID)
                    serializations.push_back(std::make_tuple(guid, ""));
                else
                    serializations.push_back(std::make_tuple(guid, serializer.Serialize(Object::FindObjectOfInstanceID(instanceID))));
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
        ObjectPtrBase Deserialize(std::string input) override
        {
            if (input.empty())
                throw std::exception("序列化数据为空");

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

            //获取目标Guid信息
            const Guid rootGuid = std::get<0>(serializations[0]);
            ObjectGuid::SignMainGuid(rootGuid);

            //设置反序列化物体的方式（将Guid转为物体引用）
            std::unordered_map<Guid, std::vector<ObjectPtrBase*>> references;
            objectImporter.template SetTransferFunc<ObjectPtrBase>([&](ObjectPtrBase& value)
            {
                Guid guid;
                objectImporter.TransferValue(guid);

                if (guid.empty())return; //空引用不用管
                if (references.contains(guid) == false)references[guid] = {}; //引用第一次出现

                references[guid].push_back(&value);
            });

            //反序列化
            std::unordered_map<Guid, ObjectPtrBase> objects;
            for (auto& [guid,data] : serializations)
            {
                //已加载的前置资源
                if (ObjectGuid::IsMainGuid(guid) && ObjectGuid::GetInstanceID(guid) != 0 && guid != rootGuid)
                    continue;

                ObjectPtrBase object = nullptr;
                if (data.empty() == false)
                {
                    object = static_cast<Object*>(serializer.Deserialize(data));
                    if (ObjectGuid::GetInstanceID(guid) == 0)
                        ObjectGuid::SetInstanceID(guid, object->GetInstanceID());
                    else //重新导入
                        Object::ReplaceObject(object, ObjectGuid::GetInstanceID(guid));
                }
                else //未加载的前置资源
                {
                    auto& fallbacks = GetDeserializeFallback();
                    for (auto& fallback : fallbacks)
                    {
                        object = fallback(guid);
                        if (object.IsNotNull())break;
                    }
                }

                if (object.IsNull())throw std::exception("无法序列化物体");
                objects[guid] = object;
            }

            //链接引用关系
            for (const auto& reference : references)
            {
                const ObjectPtrBase object = Object::FindObjectOfInstanceID(ObjectGuid::GetInstanceID(reference.first));
                for (const auto objectPtr : reference.second)
                    *objectPtr = object;
            }


            return objects[rootGuid];
        }
        ObjectPtrBase Clone(ObjectPtrBase input) override
        {
            Object* inputObject = input.ToObjectBase();
            if (inputObject == nullptr)
                throw std::exception("目标不存在");

            objectExporter.template SetTransferFunc<ObjectPtrBase>([&](const ObjectPtrBase& value)
            {
                int instanceID = value.GetInstanceID();
                objectExporter.template TransferValue<int>(instanceID);
            });

            objectImporter.template SetTransferFunc<ObjectPtrBase>([&](ObjectPtrBase& value)
            {
                int instanceID;
                objectImporter.template TransferValue<int>(instanceID);
                value = Object::FindObjectOfInstanceID(instanceID);
            });

            return static_cast<Object*>(serializer.Clone(inputObject));
        }
    private:
        Serializer serializer;
        ObjectTransferer<TExporter> objectExporter;
        ObjectTransferer<TImporter> objectImporter;
        std::function<void(Transferer&, std::string&)> transferSerialization;
    };
}
