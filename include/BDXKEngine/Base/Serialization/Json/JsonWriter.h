#pragma once
#include "JsonTransferrer.h"

namespace BDXKEngine
{
    class JsonWriter : public JsonTransferrer
    {
    public:
        JsonWriter(rapidjson::Document& buffer);

        TransferDirection GetTransferDirection() override;
    protected:
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
        void TransferValue(ISerializable& value) override;
        void TransferValue(std::vector<ObjectPtrBase>& vector) override;
        void TransferValue(char* source, int size) override;
    public:
    };
}
