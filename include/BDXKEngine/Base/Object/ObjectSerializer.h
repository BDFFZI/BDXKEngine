#pragma once
#include <queue>
#include "ObjectTransferer.h"
#include "BDXKEngine/Base/Reflection/Reflection.h"
#include "BDXKEngine/Base/Serialization/Serializer.h"
#include "Guid/Guid.h"

namespace BDXKEngine
{
    class GuidDatabase
    {
    public:
        std::unordered_map<Guid, std::string> GetSerializations();
        std::unordered_map<Guid, ObjectPtrBase> GetObjectPtrBases();

        bool GetOrGenerateGuid(int instanceID, Guid& guid);
        void SetSerialization(const Guid& guid, const std::string& serialization);
        void SetObjectPtrBase(const Guid& guid, const ObjectPtrBase& objectPtrBase);
    private:
        std::unordered_map<int, Guid> objectToGuid;
        std::unordered_map<Guid, std::string> guidToSerialization;
        std::unordered_map<Guid, ObjectPtrBase> guidToObjectPtrBase;
    };


    template <typename TExporter, typename TImporter>
    class ObjectSerializer : public Serializer
    {
    public:
        ObjectSerializer(): Serializer(objectImporter, objectExporter)
        {
        }

        std::string Serialize(Reflective* input) override
        {
            const Object* inputObject = dynamic_cast<Object*>(input);
            if (inputObject == nullptr)throw std::exception("只允许序列化Object物体");
            const int inputInstanceID = inputObject->GetInstanceID();
            if (Object::FindObjectOfInstanceID(inputInstanceID) == nullptr)throw std::exception("目标物体不存在");

            database = {};
            std::queue<int> instanceIDs;

            objectExporter.template SetTransferFunc<ObjectPtrBase>([&instanceIDs,this](ObjectPtrBase& value)
            {
                const int instanceID = value.GetInstanceID();
                if (Object::FindObjectOfInstanceID(instanceID) == nullptr)
                {
                    std::string nullSign = {};
                    objectExporter.TransferValue(nullSign);
                }
                else
                {
                    Guid guid; //第一次出现的物体引用，需要将该物体序列化
                    if (database.GetOrGenerateGuid(instanceID, guid) == false)
                        instanceIDs.push(instanceID);

                    objectExporter.TransferValue(guid);
                }
            });

            //分别序列化所有物体
            instanceIDs.push(inputInstanceID);
            while (instanceIDs.size() != 0)
            {
                const int currentInstanceID = instanceIDs.front();
                Guid currentGuid;
                database.GetOrGenerateGuid(currentInstanceID, currentGuid);
                database.SetSerialization(currentGuid, Serializer::Serialize(Object::FindObjectOfInstanceID(currentInstanceID)));

                instanceIDs.pop();
            }

            //合并序列化结果
            const std::unordered_map<std::string, std::string> serializations = database.GetSerializations();
            int serializationsCount = static_cast<int>(serializations.size());
            objectExporter.TransferField("serializationsCount", serializationsCount);
            int index = 0;
            for (const auto& item : serializations)
            {
                Guid target = item.first;
                std::string data = item.second;

                std::string itemName = "serialization_" + std::to_string(index);
                objectExporter.PushPath(itemName);
                objectExporter.TransferField("target", target);
                objectExporter.TransferField("data", data);
                objectExporter.PopPath(itemName);

                index++;
            }

            std::string result;
            objectExporter.Reset(result);
            return result;
        }
        Reflective* Deserialize(std::string input) override
        {
            database = {};
            std::map<Guid, std::vector<ObjectPtrBase*>> references;

            objectImporter.template SetTransferFunc<ObjectPtrBase>([&](ObjectPtrBase& value)
            {
                Guid guid;
                objectImporter.TransferValue(guid);

                if (guid.empty())return; //空引用不用管
                if (references.count(guid) == 0)references[guid] = {}; //引用第一次出现

                references[guid].push_back(&value);
            });

            //读取序列化数据
            objectImporter.Reset(input);
            int serializationsCount;
            objectImporter.TransferField("serializationsCount", serializationsCount);
            for (int index = 0; index < serializationsCount; index++)
            {
                Guid target = {};
                std::string data = {};

                std::string itemName = "serialization_" + std::to_string(index);
                objectImporter.PushPath(itemName);
                objectImporter.TransferField("target", target);
                objectImporter.TransferField("data", data);
                objectImporter.PopPath(itemName);

                database.SetSerialization(target, data);
            }

            //分别反序列化所有数据
            auto serializations = database.GetSerializations();
            for (const auto& serialization : serializations)
            {
                Guid target = serialization.first;
                std::string data = serialization.second;

                Object* object = dynamic_cast<Object*>(Serializer::Deserialize(data));
                database.SetObjectPtrBase(target, Object::InstantiateNoAwake(object));
            }

            //链接引用关系
            auto objectPtrBases = database.GetObjectPtrBases();
            for (const auto& reference : references)
            {
                Guid guid = reference.first;
                ObjectPtrBase object = objectPtrBases[guid];
                for (auto objectPtr : reference.second)
                    *objectPtr = object;
            }

            return objectPtrBases.begin()->second.ToObjectBase();
        }
        Reflective* Clone(Reflective* input) override
        {
            const Object* inputObject = dynamic_cast<Object*>(input);
            if (inputObject == nullptr)throw std::exception("只允许序列化Object物体");
            const int inputInstanceID = inputObject->GetInstanceID();
            if (Object::FindObjectOfInstanceID(inputInstanceID) == nullptr)throw std::exception("目标物体不存在");


            std::unordered_map<int, ObjectPtrBase> cloneObjects;
            if (inputObject->IsRunning())
            {
                objectExporter.template SetTransferFunc<ObjectPtrBase>([&](ObjectPtrBase& value)
                {
                    int instanceID = value.GetInstanceID();
                    objectExporter.TransferValue(instanceID);
                });
            }
            else //深拷贝
            {
                objectExporter.template SetTransferFunc<ObjectPtrBase>([&](ObjectPtrBase& value)
                {
                    int instanceID = value.GetInstanceID();
                    objectExporter.TransferValue(instanceID);
                    cloneObjects[instanceID] = {};
                });
            }

            //导出数据
            input->Transfer(objectExporter);
            std::string data;
            objectExporter.Reset(data);

            for (auto& item : cloneObjects)
            {
                item.second = Object::InstantiateNoAwake(
                    static_cast<Object*>(Serializer::Clone(Object::FindObjectOfInstanceID(item.first)))
                );
            }

            if (inputObject->IsRunning())
            {
                objectImporter.template SetTransferFunc<ObjectPtrBase>([&](ObjectPtrBase& value)
                {
                    int instanceID;
                    objectImporter.TransferValue(instanceID);
                    value = Object::FindObjectOfInstanceID(instanceID);
                });
            }
            else
            {
                objectImporter.template SetTransferFunc<ObjectPtrBase>([&](ObjectPtrBase& value)
                {
                    int instanceID;
                    objectImporter.TransferValue(instanceID);
                    value = cloneObjects[instanceID];
                });
            }

            //导入数据
            objectImporter.Reset(data);
            Reflective* output = Reflection::GetReflection(input).GetConstruction();
            output->Transfer(objectImporter);

            return output;
        }
    private:
        GuidDatabase database;
        ObjectTransferer<TExporter> objectExporter;
        ObjectTransferer<TImporter> objectImporter;
    };
}
