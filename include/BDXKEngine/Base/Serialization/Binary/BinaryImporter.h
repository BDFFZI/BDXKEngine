#pragma once
#include <sstream>
#include "../Importer.h"

namespace BDXKEngine
{
    class BinaryImporter : public Importer
    {
    public:
        void Import(std::string& data) override;
    protected:
        void ReadStreamTo(char* value, int size);
        template <typename TValue>
        void ReadStreamTo(TValue& value)
        {
            ReadStreamTo(reinterpret_cast<char*>(&value), sizeof(TValue));
        }

        CustomTransferFunc(int, ReadStreamTo)
        CustomTransferFunc(float, ReadStreamTo)
        CustomTransferFunc(bool, ReadStreamTo)

        void TransferString(std::string& value);
        CustomTransferFunc(std::string, TransferString)

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
    private:
        std::stringstream stream;
    };
}
