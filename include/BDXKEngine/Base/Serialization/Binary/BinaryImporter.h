#pragma once
#include <sstream>
#include "BDXKEngine/Base/Reflection/Transferer.h"
#include "BDXKEngine/Base/Serialization/Core/IOTransferer.h"

namespace BDXKEngine
{
    class BinaryImporter : public IOTransferer
    {
    public:
        bool IsImporter() override;
        void Reset(std::string& data) override;
    protected:
        void ReadStreamTo(char* value, int size);
        template <typename TValue>
        void ReadStreamTo(TValue& value)
        {
            ReadStreamTo(reinterpret_cast<char*>(&value), sizeof(TValue));
        }

        void TransferString(std::string& value);
        void TransferBytes(std::vector<char>& value);
    private:
        std::stringstream stream;

        CustomTransferFunc(int, ReadStreamTo)
        CustomTransferFunc(float, ReadStreamTo)
        CustomTransferFunc(bool, ReadStreamTo)
        CustomTransferFunc(std::string, TransferString)
        CustomTransferFunc(std::vector<char>, TransferBytes)
    };
}
