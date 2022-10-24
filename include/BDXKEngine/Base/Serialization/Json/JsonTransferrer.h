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
        rapidjson::MemoryPoolAllocator<>& GetAllocator() const;
        void PushNode(rapidjson::Value* node);
        void PopNode();

    private:
        rapidjson::Document& document;
        std::stack<rapidjson::Value*> nodes;
    };
}
