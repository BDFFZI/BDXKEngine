#pragma once
#include <sstream>
#include "BDXKEngine/Base/Reflection/Transferer.h"
#include "BDXKEngine/Base/Serialization/Core/IOTransferer.h"

namespace BDXKEngine
{
    class BinaryExporter : public IOTransferer
    {
    public:
        bool IsImporter() override;
        void Reset(std::string& data) override;
    protected:
        void WriteStreamFrom(const char* value, int size);
        template <typename TValue>
        void WriteStreamFrom(TValue& value)
        {
            WriteStreamFrom(reinterpret_cast<char*>(&value), sizeof(TValue));
        }

        void TransferString(const std::string& value);
        void TransferBytes(const std::vector<char>& value);
    private:
        std::stringstream stream;

        CustomTransferFunc(int, WriteStreamFrom)
        CustomTransferFunc(float, WriteStreamFrom)
        CustomTransferFunc(bool, WriteStreamFrom)
        CustomTransferFunc(std::string, TransferString)
        CustomTransferFunc(std::vector<char>, TransferBytes)
    };
}
