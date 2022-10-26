#pragma once
#include "JsonTransferrer.h"
#include "BDXKEngine/Base/Object/ObjectManager.h"

namespace BDXKEngine
{
    class JsonWriter : public JsonTransferrer, ObjectManager
    {
    public:
        JsonWriter(rapidjson::Document& buffer);

        TransferDirection GetTransferDirection() override;
        void PushPath(const std::string& key) override;
        void PopPath(std::string& key) override;
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
        void TransferValue(Serializable& value) override;
        void TransferValue(std::vector<ObjectPtrBase>& vector) override;
        void TransferValue(char* source, int size) override;
    public:
    };
}
