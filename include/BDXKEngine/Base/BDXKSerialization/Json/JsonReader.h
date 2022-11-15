#pragma once
#include "Jsontransferer.h"

namespace BDXKEngine
{
    class JsonReader : public Jsontransferer
    {
    public:
        JsonReader(rapidjson::Document& buffer);
        
        TransferDirection GetTransferDirection() override;
    protected:
        void PushPath(const std::string& key) override;
        void PopPath(std::string& key) override;
        
        void TransferValue(int& value) override;
        void TransferValue(float& value) override;
        void TransferValue(bool& value) override;
        void TransferValue(Vector2& value) override;
        void TransferValue(Vector3& value) override;
        void TransferValue(Vector4& value) override;
        void TransferValue(Color& value) override;
        void TransferValue(Rect& value) override;
        void TransferValue(std::string& value) override;
        void TransferValue(ObjectPtrBase& value) override;
        void TransferValue(Serialization& value) override;
        void TransferValue(char* source, int size) override;
    public:
    };
}
