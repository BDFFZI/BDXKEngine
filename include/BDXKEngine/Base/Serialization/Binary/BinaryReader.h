#pragma once
#include "BinaryTransferrer.h"
#include "BDXKEngine/Base/Object/ObjectManager.h"

namespace BDXKEngine
{
    class BinaryReader : public BinaryTransferrer, ObjectManager
    {
    public:
        BinaryReader(std::iostream& buffer);

        TransferDirection GetTransferDirection() override { return TransferDirection::Input; }
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
        void Read(TValue* value)
        {
            stream.read(reinterpret_cast<char*>(value), sizeof(TValue));
        }
    };
}
