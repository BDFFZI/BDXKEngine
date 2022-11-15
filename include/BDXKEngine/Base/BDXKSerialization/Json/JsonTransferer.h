#pragma once
#include <stack>
#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>
#include "BDXKEngine/Base/Reflection/Transferer.h"

namespace BDXKEngine
{
    class JsonTransferer : public Transferer
    {
    protected:
        JsonTransferer();

        rapidjson::Document& GetDocument();
        rapidjson::MemoryPoolAllocator<>& GetAllocator();
        rapidjson::Value& GetCurrentNode();
        
        void PushNode(rapidjson::Value* node);
        void PopNode();
        void ResetNodes();
    private:
        rapidjson::Document document;
        std::stack<rapidjson::Value*> nodes;
    };
}
