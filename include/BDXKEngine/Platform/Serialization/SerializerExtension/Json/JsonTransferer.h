#pragma once
#include <stack>
#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>
#include "BDXKEngine/Base/Serializer/Core/IOTransferer.h"

namespace BDXKEngine
{
    class JsonTransferer : public IOTransferer
    {
    public:
        virtual void TransferJson(std::string key, std::string& value) =0;

    protected:
        static void Export(rapidjson::Document& document, std::string& json);
        static void Import(rapidjson::Document& document, std::string& json);

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
