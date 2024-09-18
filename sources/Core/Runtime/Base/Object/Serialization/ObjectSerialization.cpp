#include "ObjectSerialization.h"
#include <filesystem>

namespace BDXKEngine
{
    void ObjectSerialization::AddDeserializeFallback(const std::function<ObjectPtrBase(const xg::Guid& guid)>& fallback)
    {
        deserializeFallbacks.push_back(fallback);
    }
    ObjectSerializer<JsonReader, JsonWriter> ObjectSerialization::CreateJsonSerializer()
    {
        return ObjectSerializer<JsonReader, JsonWriter>{
            [](Transferer& transferer, std::string& serialization)
            {
                dynamic_cast<JsonTransferer&>(transferer).TransferJson("data", serialization);
                if (serialization == "null")serialization = "";
            }
        };
    }
    ObjectSerializer<BinaryReader, BinaryWriter> ObjectSerialization::CreateBinarySerializer()
    {
        return ObjectSerializer<BinaryReader, BinaryWriter>{};
    }

    ObjectPtrBase ObjectSerialization::Load(const std::filesystem::path& path, ObjectSerializerBase& serializer,
                                            const xg::Guid& persistent, const bool instantiate)
    {
        //反序列化
        ObjectPtrBase objectPtr = serializer.Deserialize(ReadFile(path), persistent, DeserializeFallback);
        
        if (instantiate)
            Object::AwakeObject(objectPtr);
        return objectPtr;
    }
    void ObjectSerialization::Save(const std::filesystem::path& path, const ObjectPtrBase& objectPtr, ObjectSerializerBase& serializer,
                                   const xg::Guid& persistent)
    {
        if (const auto parentPath = path.parent_path(); parentPath.empty() == false && exists(parentPath) == false)
            throw std::exception("目录不存在");

        //序列化
        WriteFile(path, serializer.Serialize(objectPtr, persistent));
    }
    ObjectPtrBase ObjectSerialization::Clone(const ObjectPtrBase& objectPtr, const bool instantiate)
    {
        if (objectPtr.IsNull())
            throw std::exception("实例化的物体为空");

        //克隆
        auto binarySerializer = CreateBinarySerializer();
        ObjectPtrBase instance = binarySerializer.Clone(objectPtr);

        if (instantiate)
            Object::AwakeObject(instance);

        return instance;
    }


    bool ObjectSerialization::IsPersistent(const ObjectPtrBase& object)
    {
        return GetPersistent(object).isValid();
    }
    xg::Guid ObjectSerialization::GetPersistent(const ObjectPtrBase& object)
    {
        return ObjectRef::GetObjectRef(object.GetObjectID()).guid;
    }
    void ObjectSerialization::MarkPersistent(const ObjectPtrBase& object, const xg::Guid guid)
    {
        const ObjectRef objectRef = {guid, ObjectRef::GenerateFileID(guid)};
        ObjectRef::Add(objectRef, object.GetObjectID());
    }
    void ObjectSerialization::UnMarkPersistent(const ObjectPtrBase& object)
    {
        ObjectRef::Remove(object.GetObjectID());
    }
    ObjectPtrBase ObjectSerialization::DeserializeFallback(const ObjectRef& other)
    {
        for (auto& fallback : deserializeFallbacks)
        {
            // ReSharper disable once CppDeclaratorNeverUsed 保证不触发指针回收
            ObjectPtrBase temp = fallback(other.guid);
            const ID objectID = ObjectRef::GetObjectID(other);
            const Object* target = Object::FindObjectOfObjectID(objectID);
            if (target != nullptr)
                return target;
        }

        return {};
    }
}
