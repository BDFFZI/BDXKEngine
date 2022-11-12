#pragma once
#include "BinaryTransferer.h"

namespace BDXKEngine
{
    class BinaryReader : public BinaryTransferer
    {
    public:
        BinaryReader(const std::shared_ptr<std::iostream>& stream);

    protected:
        CustomTransferFunc(int, ReadStreamTo, 0)
        CustomTransferFunc(float, ReadStreamTo, 1)
        CustomTransferFunc(bool, ReadStreamTo, 2)

        void TransferString(std::string& value);
        CustomTransferFunc(std::string, TransferString, 0)

        // void TransferValue(int& value) override;
        // void TransferValue(float& value) override;
        // void TransferValue(bool& value) override;
        // void TransferValue(Vector2& value) override;
        // void TransferValue(Vector3& value) override;
        // void TransferValue(Vector4& value) override;
        // void TransferValue(Color& value) override;
        // void TransferValue(Rect& value) override;
        // void TransferValue(ObjectPtrBase& value) override;
        // void TransferValue(Serialization& value) override;
        // void TransferValue(char* source, int size) override;
    };
}
