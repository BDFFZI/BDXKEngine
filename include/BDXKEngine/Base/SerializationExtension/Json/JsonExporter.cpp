#include "JsonExporter.h"

#include <iostream>
#include <rapidjson/prettywriter.h>

namespace BDXKEngine
{
    void JsonExporter::TransferJson(std::string key, std::string& value)
    {
        auto& allocator = GetAllocator();
        rapidjson::GenericValue<rapidjson::UTF8<>> keystring = {key.c_str(), static_cast<rapidjson::SizeType>(key.length()), allocator};

        rapidjson::Document document = {&allocator};
        Import(document, value);

        GetCurrentNode().AddMember(keystring, document, GetAllocator());
    }
    void JsonExporter::Reset(std::string& data)
    {
        Export(GetDocument(), data);
        ResetNodes();
    }
    void JsonExporter::PushPath(const std::string& key)
    {
        auto& allocator = GetAllocator();
        rapidjson::GenericValue<rapidjson::UTF8<>> keystring(key.c_str(), static_cast<rapidjson::SizeType>(key.length()), allocator);

        auto& currentNode = GetCurrentNode();
        if (currentNode.IsObject() == false)currentNode.SetObject();
        currentNode.AddMember(keystring, keystring, allocator);

        PushNode(&currentNode[key.c_str()]);
    }
    void JsonExporter::PopPath(std::string& key)
    {
        PopNode();
    }
    void JsonExporter::TransferInt(int& value)
    {
        GetCurrentNode().SetInt(value);
    }
    void JsonExporter::TransferFloat(float& value)
    {
        GetCurrentNode().SetFloat(value);
    }
    void JsonExporter::TransferBool(bool& value)
    {
        GetCurrentNode().SetBool(value);
    }
    void JsonExporter::TransferVector2(Vector2& value)
    {
        auto& node = GetCurrentNode().SetArray();

        auto& allocator = GetAllocator();
        node.PushBack(value.x, allocator);
        node.PushBack(value.y, allocator);
    }
    void JsonExporter::TransferVector3(Vector3& value)
    {
        auto& node = GetCurrentNode().SetArray();

        auto& allocator = GetAllocator();
        node.PushBack(value.x, allocator);
        node.PushBack(value.y, allocator);
        node.PushBack(value.z, allocator);
    }
    void JsonExporter::TransferVector4(Vector4& value)
    {
        auto& node = GetCurrentNode().SetArray();

        auto& allocator = GetAllocator();
        node.PushBack(value.x, allocator);
        node.PushBack(value.y, allocator);
        node.PushBack(value.z, allocator);
        node.PushBack(value.w, allocator);
    }
    void JsonExporter::TransferColor(Color& value)
    {
        auto& node = GetCurrentNode().SetArray();

        auto& allocator = GetAllocator();
        node.PushBack(value.r, allocator);
        node.PushBack(value.g, allocator);
        node.PushBack(value.b, allocator);
        node.PushBack(value.a, allocator);
    }
    void JsonExporter::TransferRect(Rect& value)
    {
        auto& node = GetCurrentNode().SetArray();

        auto& allocator = GetAllocator();
        node.PushBack(value.x, allocator);
        node.PushBack(value.y, allocator);
        node.PushBack(value.width, allocator);
        node.PushBack(value.height, allocator);
    }
    void JsonExporter::TransferString(std::string& value)
    {
        GetCurrentNode().SetString(value.c_str(), static_cast<rapidjson::SizeType>(value.size()), GetAllocator());
    }
}
