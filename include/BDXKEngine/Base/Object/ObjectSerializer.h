#pragma once
#include <objbase.h>
#include <queue>
#include "ObjectTransferer.h"
#include "BDXKEngine/Base/Reflection/Reflection.h"
#include "BDXKEngine/Base/Serialization/Serializer.h"
#include "BDXKEngine/Base/Serialization/Binary/BinaryExporter.h"
#include "BDXKEngine/Base/Serialization/Binary/BinaryImporter.h"

namespace BDXKEngine
{
    class GuidDatabase
    {
    public:
        std::map<std::string, std::string> GetSerializations()
        {
            return guidToSerialization;
        }

        std::string NewGuid()
        {
            GUID nativeGuid = {};
            CoCreateGuid(&nativeGuid);

            char data[64];
            const int size = sprintf_s(
                data, "{%08X-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X}",
                nativeGuid.Data1, nativeGuid.Data2, nativeGuid.Data3,
                nativeGuid.Data4[1], nativeGuid.Data4[0], nativeGuid.Data4[7], nativeGuid.Data4[6],
                nativeGuid.Data4[5], nativeGuid.Data4[4], nativeGuid.Data4[3], nativeGuid.Data4[2]
            );

            std::string guid = std::string(data);
            guid.resize(size);

            return guid;
        }
        bool GetOrGenerateGuid(int instanceID, std::string& guid)
        {
            const auto guidItem = instanceIDToGuid.find(instanceID);
            if (guidItem == instanceIDToGuid.end())
            {
                guid = NewGuid();
                instanceIDToGuid[instanceID] = guid;
                return false;
            }
            else
            {
                guid = guidItem->second;
                return true;
            }
        }
        void SetSerialization(const std::string& guid, const std::string& serialization)
        {
            guidToSerialization[guid] = serialization;
        }

        std::string InstanceIDToGuid(int instanceID)
        {
            const auto guidItem = instanceIDToGuid.find(instanceID);
            return guidItem == instanceIDToGuid.end()
                       ? ""
                       : guidItem->second;
        }
        int GuidToInstanceID(const std::string& guid)
        {
            const auto instanceIDItem = guidToInstanceID.find(guid);
            return instanceIDItem == guidToInstanceID.end()
                       ? 0
                       : instanceIDItem->second;
        }
    private:
        std::map<int, std::string> instanceIDToGuid;
        std::map<std::string, int> guidToInstanceID;
        std::map<std::string, std::string> guidToSerialization;
    };


    template <typename TImporter = BinaryImporter, typename TExporter = BinaryExporter>
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

            GuidDatabase database;
            std::queue<int> instanceIDs;

            objectExporter.template SetTransferFunc<ObjectPtrBase>([&database,&instanceIDs,this](ObjectPtrBase& value)
            {
                const int instanceID = value.GetInstanceID();
                if (Object::FindObjectOfInstanceID(instanceID) == nullptr)
                {
                    std::string nullSign = "nullptr";
                    objectExporter.TransferValue(nullSign);
                }
                else
                {
                    std::string guid; //第一次出现的物体引用，需要将该物体序列化
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
                std::string currentGuid;
                database.GetOrGenerateGuid(currentInstanceID, currentGuid);
                database.SetSerialization(currentGuid, Serializer::Serialize(Object::FindObjectOfInstanceID(currentInstanceID)));

                instanceIDs.pop();
            }

            //合并序列化结果
            for (const auto& item :  database.GetSerializations())
            {
                std::string objectString = item.first;
                std::string serialization = item.second;
                
                objectExporter.TransferValue(objectString);
                objectExporter.TransferValue(serialization);
            }

            std::string result;
            objectExporter.Export(result);
            return result;
        }
        Reflective* Deserialize(std::string input) override
        {
            objectImporter.template SetTransferFunc<ObjectPtrBase>([&](ObjectPtrBase& value)
            {
                int instanceID = value.GetInstanceID();
                objectImporter.TransferValue(instanceID);
                value = {Object::FindObjectOfInstanceID(instanceID)};
            });

            //TODO 将输入分割还原成多份序列化，并将其实例化后并连接引用关系

            return Serializer::Deserialize(input);
        }
        Reflective* Clone(Reflective* input) override
        {
            //初始化传输器
            objectExporter.template SetTransferFunc<ObjectPtrBase>([&](ObjectPtrBase& value)
            {
                int instanceID = value.GetInstanceID();
                objectExporter.TransferValue(instanceID);
                value = {Object::FindObjectOfInstanceID(instanceID)};
            });
            objectImporter.template SetTransferFunc<ObjectPtrBase>([&](ObjectPtrBase& value)
            {
                int instanceID = value.GetInstanceID();
                objectImporter.TransferValue(instanceID);
                value = {Object::FindObjectOfInstanceID(instanceID)};
            });

            //导出数据
            input->Transfer(objectExporter);
            std::string data;
            objectExporter.Export(data);

            //导入数据
            objectImporter.Import(data);
            Reflective* output = Reflection::GetReflection(input).GetConstruction();
            output->Transfer(objectImporter);

            return output;
        }
    private:
        ObjectTransferer<TImporter> objectImporter;
        ObjectTransferer<TExporter> objectExporter;
    };
}
