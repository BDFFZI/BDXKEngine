#pragma once
#include <sstream>
#include "BDXKEngine/Base/Reflection/Transferer.h"

namespace BDXKEngine
{
    class BinaryExporter : public Transferer
    {
    public:
        void Reset(std::string& data) override;
    protected:
        void WriteStreamFrom(const char* value, int size);
        template <typename TValue>
        void WriteStreamFrom(TValue& value)
        {
            WriteStreamFrom(reinterpret_cast<char*>(&value), sizeof(TValue));
        }

        void TransferString(const std::string& value);
    private:
        std::stringstream stream;

        CustomTransferFunc(int, WriteStreamFrom)
        CustomTransferFunc(float, WriteStreamFrom)
        CustomTransferFunc(bool, WriteStreamFrom)
        CustomTransferFunc(std::string, TransferString)
    };
}
