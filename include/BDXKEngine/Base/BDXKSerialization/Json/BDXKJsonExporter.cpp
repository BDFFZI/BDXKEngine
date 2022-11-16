#include "BDXKJsonExporter.h"

#include <iostream>
#include <rapidjson/prettywriter.h>

namespace BDXKEngine
{
    void BDXKJsonExporter::TransferJson(std::string key, std::string& value)
    {
        auto& allocator = GetAllocator();
        rapidjson::GenericValue<rapidjson::UTF8<>> keystring = {key.c_str(), static_cast<rapidjson::SizeType>(key.length()), allocator};

        rapidjson::Document document = {&allocator};
        Import(document, value);

        GetCurrentNode().AddMember(keystring, document, GetAllocator());
    }
    void BDXKJsonExporter::Reset(std::string& data)
    {
        Export(GetDocument(), data);
        ResetNodes();
    }
    void BDXKJsonExporter::PushPath(const std::string& key)
    {
        auto& allocator = GetAllocator();
        rapidjson::GenericValue<rapidjson::UTF8<>> keystring(key.c_str(), static_cast<rapidjson::SizeType>(key.length()), allocator);

        auto& currentNode = GetCurrentNode();
        if (currentNode.IsObject() == false)currentNode.SetObject();
        currentNode.AddMember(keystring, keystring, allocator);

        PushNode(&currentNode[key.c_str()]);
    }
    void BDXKJsonExporter::PopPath(std::string& key)
    {
        PopNode();
    }
    void BDXKJsonExporter::TransferInt(int& value)
    {
        GetCurrentNode().SetInt(value);
    }
    void BDXKJsonExporter::TransferFloat(float& value)
    {
        GetCurrentNode().SetFloat(value);
    }
    void BDXKJsonExporter::TransferBool(bool& value)
    {
        GetCurrentNode().SetBool(value);
    }
    void BDXKJsonExporter::TransferVector2(Vector2& value)
    {
        auto& node = GetCurrentNode().SetArray();

        auto& allocator = GetAllocator();
        node.PushBack(value.x, allocator);
        node.PushBack(value.y, allocator);
    }
    void BDXKJsonExporter::TransferVector3(Vector3& value)
    {
        auto& node = GetCurrentNode().SetArray();

        auto& allocator = GetAllocator();
        node.PushBack(value.x, allocator);
        node.PushBack(value.y, allocator);
        node.PushBack(value.z, allocator);
    }
    void BDXKJsonExporter::TransferVector4(Vector4& value)
    {
        auto& node = GetCurrentNode().SetArray();

        auto& allocator = GetAllocator();
        node.PushBack(value.x, allocator);
        node.PushBack(value.y, allocator);
        node.PushBack(value.z, allocator);
        node.PushBack(value.w, allocator);
    }
    void BDXKJsonExporter::TransferColor(Color& value)
    {
        auto& node = GetCurrentNode().SetArray();

        auto& allocator = GetAllocator();
        node.PushBack(value.r, allocator);
        node.PushBack(value.g, allocator);
        node.PushBack(value.b, allocator);
        node.PushBack(value.a, allocator);
    }
    void BDXKJsonExporter::TransferRect(Rect& value)
    {
        auto& node = GetCurrentNode().SetArray();

        auto& allocator = GetAllocator();
        node.PushBack(value.x, allocator);
        node.PushBack(value.y, allocator);
        node.PushBack(value.width, allocator);
        node.PushBack(value.height, allocator);
    }
    void BDXKJsonExporter::TransferString(std::string& value)
    {
        GetCurrentNode().SetString(value.c_str(), static_cast<rapidjson::SizeType>(value.size()), GetAllocator());
    }
}
