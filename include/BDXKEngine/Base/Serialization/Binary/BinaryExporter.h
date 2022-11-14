#pragma once
#include <sstream>
#include "../Exporter.h"

namespace BDXKEngine
{
    class BinaryExporter : public Exporter
    {
    public:
        void Export(std::string& data) override;
    protected:
        void WriteStreamFrom(const char* value, int size);
        template <typename TValue>
        void WriteStreamFrom(TValue& value)
        {
            WriteStreamFrom(reinterpret_cast<char*>(&value), sizeof(TValue));
        }

        CustomTransferFunc(int, WriteStreamFrom)
        CustomTransferFunc(float, WriteStreamFrom)
        CustomTransferFunc(bool, WriteStreamFrom)

        void TransferString(const std::string& value);
        CustomTransferFunc(std::string, TransferString)

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
    private:
        std::stringstream stream;
    };
}
