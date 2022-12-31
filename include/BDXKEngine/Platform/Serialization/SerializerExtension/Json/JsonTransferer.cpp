#include "JsonTransferer.h"
#include <rapidjson/prettywriter.h>

namespace BDXKEngine
{
    void JsonTransferer::Export(rapidjson::Document& document, std::string& json)
    {
        rapidjson::StringBuffer buffer{};
        rapidjson::PrettyWriter writer{buffer};
        document.Accept(writer);
        document.Parse("{}");
        json = buffer.GetString();
    }
    void JsonTransferer::Import(rapidjson::Document& document, std::string& json)
    {
        document.Parse(json.c_str());
    }

    JsonTransferer::JsonTransferer()
    {
        document.Parse("{}");
        nodes.push(&document);
    }

    rapidjson::Document& JsonTransferer::GetDocument()
    {
        return document;
    }
    rapidjson::MemoryPoolAllocator<>& JsonTransferer::GetAllocator()
    {
        return document.GetAllocator();
    }
    rapidjson::Value& JsonTransferer::GetCurrentNode()
    {
        return *nodes.top();
    }
    bool JsonTransferer::HasCurrentNode()
    {
        return nodes.top() != nullptr;
    }

    void JsonTransferer::PushNode(rapidjson::Value* node)
    {
        nodes.push(node);
    }
    void JsonTransferer::PopNode()
    {
        nodes.pop();
    }
    void JsonTransferer::ResetNodes()
    {
        nodes = {};
        nodes.push(&document);
    }
}
