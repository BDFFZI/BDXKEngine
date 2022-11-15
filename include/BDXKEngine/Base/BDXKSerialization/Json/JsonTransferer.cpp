#include "JsonTransferer.h"
#include <rapidjson/prettywriter.h>

namespace BDXKEngine
{
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
