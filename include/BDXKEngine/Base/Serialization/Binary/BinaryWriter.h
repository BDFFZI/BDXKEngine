#pragma once
#include "BDXKEngine/Base/Object/ObjectManager.h"
#include "BinaryTransferrer.h"

namespace BDXKEngine
{
    class BinaryWriter : public BinaryTransferrer, ObjectManager
    {
    public:
        BinaryWriter(std::iostream& buffer);

        TransferDirection GetTransferDirection() override { return TransferDirection::Output; }
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
        void TransferValue(char* source, int size) override;
    private:
        template <typename TValue>
        void Write(TValue value)
        {
            stream.write(reinterpret_cast<char*>(&value), sizeof(TValue));
        }
    };
}