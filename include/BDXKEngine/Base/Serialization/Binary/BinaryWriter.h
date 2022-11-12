#pragma once
#include "BinaryTransferer.h"

namespace BDXKEngine
{
    class BinaryWriter : public BinaryTransferer
    {
    public:
        BinaryWriter(const std::shared_ptr<std::iostream>& stream);

    protected:
        CustomTransferFunc(int, WriteStreamFrom, 0)
        CustomTransferFunc(float, WriteStreamFrom, 1)
        CustomTransferFunc(bool, WriteStreamFrom, 2)

        void TransferString(const std::string& value);
        CustomTransferFunc(std::string, TransferString, 0)

        // void TransferValue(char* source, int size) override;
        // void TransferValue(float& value) override;
        // void TransferValue(bool& value) override;
        // void TransferValue(Vector2& value) override;
        // void TransferValue(Vector3& value) override;
        // void TransferValue(Vector4& value) override;
        // void TransferValue(Color& value) override;
        // void TransferValue(Rect& value) override;
        // void TransferValue(std::string& value) override;
        // void TransferValue(ObjectPtrBase& value) override;
        // void TransferValue(Serialization& value) override;
    };
}
