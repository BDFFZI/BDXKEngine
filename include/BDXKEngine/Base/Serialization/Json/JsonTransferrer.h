#pragma once
#include <stack>
#include <rapidjson/document.h>
#include "BDXKEngine/Base/Serialization/Transferrer.h"

namespace BDXKEngine
{
    class JsonTransferrer : public Transferrer
    {
    protected:
        JsonTransferrer(rapidjson::Document& buffer);

        rapidjson::Value& GetCurrentNode();
        void PushPath(const std::string& key) override;
        void PopPath(std::string& key) override;

        rapidjson::Document& document;
        std::stack<rapidjson::Value*> nodes;
    };
}
