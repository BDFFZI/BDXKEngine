#pragma once
#include <tuple>
#include "ObjectRef.h"
#include "Core/Runtime/Base/Object/Object.h"
#include "Core/Runtime/Base/Object/Transferer/ObjectPtrTransferer.h"
#include "Core/Runtime/Base/Debug/Debug.h"
#include "Core/Runtime/Base/Serializer/Serializer.h"

namespace BDXKEngine
{
    class SerializeEvent
    {
    public:
        virtual ~SerializeEvent() = default;
        virtual void OnBeforeSerialize() =0;
    };

    class DeserializeEvent
    {
    public:
        virtual ~DeserializeEvent() = default;
        virtual void OnAfterDeserialize() =0;
    };


    class ObjectSerializerBase
    {
    public:
        virtual ~ObjectSerializerBase() = default;
        virtual std::string Serialize(ObjectPtrBase input, xg::Guid guid) = 0;
        virtual ObjectPtrBase Deserialize(std::string input, xg::Guid guid,
                                          std::function<ObjectPtrBase(const ObjectRef& other)> deserializeFallback) = 0;
        virtual ObjectPtrBase Clone(ObjectPtrBase input) = 0;
    };

    template <typename TReader, typename TWriter>
    class ObjectSerializer : public ObjectSerializerBase
    {
    public:
        ObjectSerializer(const std::function<void(Transferer&, std::string&)>& transferObjectData = [
            ](Transferer& transferer, std::string& objectData)
        {
            transferer.TransferField("data", objectData);
        }):
            serializer(objectReader, objectWriter), transferObjectData(transferObjectData)
        {
        }

        std::string Serialize(ObjectPtrBase input, xg::Guid guid) override
        {
            Object* rootObject = input.ToObject();
            if (rootObject == nullptr)
                throw std::exception("目标不存在");

            //获取目标Guid信息
            const ID rootObjectID = rootObject->GetObjectID();
            ObjectRef rootObjectRef = ObjectRef::GetObjectRef(rootObjectID);
            if (rootObjectRef.guid != guid && guid.isValid())
            {
                if (rootObjectRef.guid.isValid())
                    ObjectRef::Remove(rootObjectID);
                rootObjectRef.guid = guid;
                rootObjectRef.fileID = 1000000000;
                ObjectRef::Add(rootObjectRef, rootObjectID);
            }
            ObjectRef::MarkRoot(rootObjectRef);

            //统计需要序列化的物体
            ObjectPtrTransferer objectPtrTransferer = {};
            objectPtrTransferer.TransferObjectPtr(input);
            rootObject->Transfer(objectPtrTransferer);
            const auto references = objectPtrTransferer.GetObjectIDs();

            //生成ObjectRef
            for (auto& objectID : references)
            {
                ObjectRef objectRef = ObjectRef::GetObjectRef(objectID);
                if (objectRef.IsNull()) //非序列化物体，此时需要打包到一起
                {
                    objectRef.guid = rootObjectRef.guid;
                    objectRef.fileID = ObjectRef::GenerateFileID(objectRef.guid);
                    ObjectRef::Add(objectRef, objectID);
                }
            }

            //设置序列化物体的方式（将物体引用转为Guid）
            objectWriter.template SetTransfer<ObjectPtrBase>([&](const ObjectPtrBase& value)
            {
                const ID objectID = value.GetObjectID();

                //因为之前生成过ObjectRef所以，此处只要注意为空判断就行
                ObjectRef objectRef;
                if (Object::FindObjectOfObjectID(objectID) != nullptr)
                    objectRef = ObjectRef::GetObjectRef(objectID);
                else
                    objectRef = ObjectRef::Null();

                //同组物体只要保存fileID即可
                if (objectRef.guid == rootObjectRef.guid)
                    objectRef.guid = {};

                objectWriter.TransferValueRecursive(objectRef);
            });


            //序列化
            std::vector<std::tuple<FileID, std::string>> objectData;
            for (auto& objectID : references)
            {
                ObjectRef objectRef = ObjectRef::GetObjectRef(objectID);
                if (objectRef.guid == rootObjectRef.guid) //只序列化同组的物体
                {
                    Object* target = Object::FindObjectOfObjectID(objectID);

                    //序列化回调
                    if (auto* handler = dynamic_cast<SerializeEvent*>(target); handler != nullptr)
                        handler->OnBeforeSerialize();

                    objectData.push_back(std::make_tuple(
                        objectRef.fileID,
                        serializer.Serialize(target)
                    ));
                }
            }

            //输出序列化数据
            int objectCount = static_cast<int>(objectData.size());
            objectWriter.TransferField("objectCount", objectCount);
            int index = 0;
            for (auto& [fileID,data] : objectData)
            {
                std::string itemName = "object_" + std::to_string(index);
                objectWriter.PushPath(itemName);
                objectWriter.TransferField("fileID", fileID);
                transferObjectData(objectWriter, data);
                objectWriter.PopPath(itemName);
                index++;
            }
            std::string result;
            objectWriter.Reset(result);
            return result;
        }
        ObjectPtrBase Deserialize(std::string input, xg::Guid guid,
                                  std::function<ObjectPtrBase(const ObjectRef& other)> deserializeFallback) override
        {
            if (input.empty())
                throw std::exception("序列化数据为空");

            //获取反序列化数据
            objectReader.Reset(input);
            int objectCount;
            objectReader.TransferField("objectCount", objectCount);
            std::vector<std::tuple<FileID, std::string>> objectData;
            for (int index = 0; index < objectCount; index++)
            {
                FileID fileID = 0;
                std::string data = {};

                std::string itemName = "object_" + std::to_string(index);
                objectReader.PushPath(itemName);
                objectReader.TransferField("fileID", fileID);
                transferObjectData(objectReader, data);
                objectReader.PopPath(itemName);

                objectData.emplace_back(fileID, data);
            }

            //设置反序列化物体的方式（统计相对ObjectRef的物体引用）
            std::unordered_map<ObjectRef, std::vector<ObjectPtrBase*>> references;
            objectReader.template SetTransfer<ObjectPtrBase>([&](ObjectPtrBase& value)
            {
                ObjectRef objectRef;
                objectReader.TransferValueRecursive(objectRef);

                if (objectRef.IsNull()) //空引用不用管
                    return;

                if (objectRef.guid.isValid() == false)
                    objectRef.guid = guid; //同组物体存储时guid为空，此时还原

                references[objectRef].push_back(&value);
            });

            //反序列化
            std::vector<ObjectPtrBase> allObjects = {}; //保存所有相关物体指针，防止被错误回收
            std::unordered_map<int, int> replaceObjects = {}; //待会需要重新导入的物体
            for (auto& [fileID,data] : objectData)
            {
                ObjectRef objectRef = {guid, fileID};
                ObjectPtrBase object = static_cast<Object*>(serializer.Deserialize(data));
                allObjects.push_back(object);

                if (ID oldObjectID = ObjectRef::GetObjectID(objectRef); oldObjectID != 0)
                {
                    //曾经加载过，需要覆盖
                    replaceObjects[object.GetObjectID()] = oldObjectID;
                }
                else
                {
                    ObjectRef::Add(objectRef, object->GetObjectID());
                }
            }

            //替换旧物体(必须先完全删除所有旧的，挨个替换可能出现旧物体将新物体删除的情况，比如GameObject和Component的关联删除)
            for (auto& objectID : replaceObjects | std::views::values)
                Object::DestroyObject(Object::FindObjectOfObjectID(objectID));
            for (auto& object : allObjects) //遍历allObjects是为了顺便修正指针数据
            {
                int oldObjectID = replaceObjects[object.GetObjectID()];
                if (oldObjectID != 0) //需要覆盖物体
                    Object::ReplaceObject(object, oldObjectID);
            }

            //链接引用关系
            for (auto& [objectRef,objectPtrs] : references)
            {
                ID objectID = ObjectRef::GetObjectID(objectRef);
                ObjectPtrBase object = Object::FindObjectOfObjectID(objectID);
                if (object.IsNull())
                {
                    object = deserializeFallback(objectRef);
                    if (object.IsNull())
                        Debug::LogWarning("引用丢失:" + objectRef.ToString());
                }

                for (auto& objectPtr : objectPtrs)
                    *objectPtr = object;
            }

            //反序列化回调
            for (auto& object : allObjects)
            {
                if (auto* handler = object.ToObject<DeserializeEvent>(); handler != nullptr)
                    handler->OnAfterDeserialize();
            }

            ObjectRef::MarkRoot(allObjects[0].GetObjectID());
            return allObjects[0];
        }
        ObjectPtrBase Clone(const ObjectPtrBase input) override
        {
            Object* inputObject = input.ToObject();
            if (inputObject == nullptr)
                throw std::exception("目标不存在");

            objectWriter.template SetTransfer<ObjectPtrBase>([&](const ObjectPtrBase& value)
            {
                int objectID = value.GetObjectID();
                objectWriter.template TransferValue<int>(objectID);
            });

            objectReader.template SetTransfer<ObjectPtrBase>([&](ObjectPtrBase& value)
            {
                int objectID;
                objectReader.template TransferValue<int>(objectID);
                value = Object::FindObjectOfObjectID(objectID);
            });

            if (auto* handler = dynamic_cast<SerializeEvent*>(inputObject); handler != nullptr)
                handler->OnBeforeSerialize();
            ObjectPtrBase newObject = static_cast<Object*>(serializer.Clone(inputObject));
            if (auto* handler = newObject.ToObject<DeserializeEvent>(); handler != nullptr)
                handler->OnAfterDeserialize();
            return newObject;
        }

    private:
        Serializer serializer;
        ObjectTransferer<TWriter> objectWriter;
        ObjectTransferer<TReader> objectReader;
        std::function<void(Transferer&, std::string&)> transferObjectData;
    };
}
