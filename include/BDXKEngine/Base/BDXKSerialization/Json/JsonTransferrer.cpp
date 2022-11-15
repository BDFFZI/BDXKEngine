#include "Jsontransferer.h"

namespace BDXKEngine
{
    std::string Jsontransferer::ToString() const
    {
        rapidjson::StringBuffer buffer{};
        rapidjson::PrettyWriter writer{buffer};
        document.Accept(writer);
        return buffer.GetString();
    }

    Jsontransferer::Jsontransferer(rapidjson::Document& buffer): document(buffer), nodes({&buffer})
    {
    }
    rapidjson::Value& Jsontransferer::GetCurrentNode()
    {
        return *nodes.top();
    }
    rapidjson::MemoryPoolAllocator<>& Jsontransferer::GetAllocator() const
    {
        return document.GetAllocator();
    }

    void Jsontransferer::PushNode(rapidjson::Value* node)
    {
        nodes.push(node);
    }
    void Jsontransferer::PopNode()
    {
        nodes.pop();
    }
}
