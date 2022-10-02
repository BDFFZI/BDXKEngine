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
    void JsonTransferrer::PushPath(const std::string& key)
    {
        rapidjson::Value& currentNode = GetCurrentNode();

        rapidjson::GenericValue<rapidjson::UTF8<>> keystring(key.c_str(), static_cast<rapidjson::SizeType>(key.length()));
        currentNode.AddMember(keystring, keystring, document.GetAllocator());

        nodes.push(&currentNode[key.c_str()]);
    }
    void JsonTransferrer::PopPath(std::string& key)
    {
        nodes.pop();
    }
}
