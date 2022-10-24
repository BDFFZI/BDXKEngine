#include "JsonTransferrer.h"

namespace BDXKEngine
{
    JsonTransferrer::JsonTransferrer(rapidjson::Document& buffer): document(buffer), nodes({&buffer})
    {
    }
    rapidjson::Value& JsonTransferrer::GetCurrentNode()
    {
        return *nodes.top();
    }
    rapidjson::MemoryPoolAllocator<>& JsonTransferrer::GetAllocator() const
    {
        return document.GetAllocator();
    }
    
    void JsonTransferrer::PushNode(rapidjson::Value* node)
    {
        nodes.push(node);
    }
    void JsonTransferrer::PopNode()
    {
        nodes.pop();
    }
}
