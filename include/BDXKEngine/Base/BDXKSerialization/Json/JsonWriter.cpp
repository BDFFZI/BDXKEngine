#include "JsonWriter.h"

#include "BDXKEngine/Base/Object/ObjectPtrBase.h"
#include "BDXKEngine/Base/Serialization/Serialization.h"

namespace BDXKEngine
{
    JsonWriter::JsonWriter(rapidjson::Document& buffer): Jsontransferer(buffer)
    {
    }

    TransferDirection JsonWriter::GetTransferDirection()
    {
        return TransferDirection::Output;
    }
    void JsonWriter::PushPath(const std::string& key)
    {
        auto& allocator = GetAllocator();
        rapidjson::GenericValue<rapidjson::UTF8<>> keystring(key.c_str(), static_cast<rapidjson::SizeType>(key.length()), allocator);

        auto& currentNode = GetCurrentNode();
        if (currentNode.IsObject() == false)currentNode.SetObject();
        currentNode.AddMember(keystring, keystring, allocator);

        PushNode(&currentNode[key.c_str()]);
    }
    void JsonWriter::PopPath(std::string& key)
    {
        PopNode();
    }

    void JsonWriter::TransferValue(int& value)
    {
        GetCurrentNode().SetInt(value);
    }
    void JsonWriter::TransferValue(float& value)
    {
        GetCurrentNode().SetFloat(value);
    }
    void JsonWriter::TransferValue(bool& value)
    {
        GetCurrentNode().SetBool(value);
    }
    void JsonWriter::TransferValue(Vector2& value)
    {
        auto& node = GetCurrentNode().SetArray();

        auto& allocator = GetAllocator();
        node.PushBack(value.x, allocator);
        node.PushBack(value.y, allocator);
    }
    void JsonWriter::TransferValue(Vector3& value)
    {
        auto& node = GetCurrentNode().SetArray();

        auto& allocator = GetAllocator();
        node.PushBack(value.x, allocator);
        node.PushBack(value.y, allocator);
        node.PushBack(value.z, allocator);
    }
    void JsonWriter::TransferValue(Vector4& value)
    {
        auto& node = GetCurrentNode().SetArray();

        auto& allocator = GetAllocator();
        node.PushBack(value.x, allocator);
        node.PushBack(value.y, allocator);
        node.PushBack(value.z, allocator);
        node.PushBack(value.w, allocator);
    }
    void JsonWriter::TransferValue(Color& value)
    {
        auto& node = GetCurrentNode().SetArray();

        auto& allocator = GetAllocator();
        node.PushBack(value.r, allocator);
        node.PushBack(value.g, allocator);
        node.PushBack(value.b, allocator);
        node.PushBack(value.a, allocator);
    }
    void JsonWriter::TransferValue(Rect& value)
    {
        auto& node = GetCurrentNode().SetArray();

        auto& allocator = GetAllocator();
        node.PushBack(value.x, allocator);
        node.PushBack(value.y, allocator);
        node.PushBack(value.width, allocator);
        node.PushBack(value.height, allocator);
    }
    void JsonWriter::TransferValue(std::string& value)
    {
        GetCurrentNode().SetString(value.c_str(), static_cast<rapidjson::SizeType>(value.size()), GetAllocator());
    }
    void JsonWriter::TransferValue(ObjectPtrBase& value)
    {
        std::string guid = SaveObject(value.GetInstanceID());
        TransferValue(guid);
    }
    void JsonWriter::TransferValue(Serialization& value)
    {
        value.Transfer(*this);
    }
    void JsonWriter::TransferValue(char* source, int size)
    {
    }
}
