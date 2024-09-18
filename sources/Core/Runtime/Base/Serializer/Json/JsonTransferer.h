#pragma once
#include <stack>
#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>
#include "Core/Runtime/Base/Serializer/IOTransferer.h"

namespace BDXKEngine
{
    class JsonTransferer : public IOTransferer
    {
    public:
        virtual void TransferJson(std::string key, std::string& value) =0;

        rapidjson::Document& GetDocument();
    protected:
        static void Export(rapidjson::Document& document, std::string& json);
        static void Import(rapidjson::Document& document, const std::string& json);

        JsonTransferer();

     
        rapidjson::MemoryPoolAllocator<>& GetAllocator();
        rapidjson::Value& GetCurrentNode();
        bool HasCurrentNode();

        void PushNode(rapidjson::Value* node);
        void PopNode();
        void ResetNodes();
    private:
        rapidjson::Document document;
        std::stack<rapidjson::Value*> nodes;
    };
}
