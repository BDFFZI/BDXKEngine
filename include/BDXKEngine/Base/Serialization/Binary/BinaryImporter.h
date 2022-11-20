﻿#pragma once
#include <sstream>
#include "BDXKEngine/Base/Reflection/Transferer.h"

namespace BDXKEngine
{
    class BinaryImporter : public Transferer
    {
    public:
        void Reset(std::string& data) override;
    protected:
        void ReadStreamTo(char* value, int size);
        template <typename TValue>
        void ReadStreamTo(TValue& value)
        {
            ReadStreamTo(reinterpret_cast<char*>(&value), sizeof(TValue));
        }

        void TransferString(std::string& value);
    private:
        std::stringstream stream;

        CustomTransferFunc(int, ReadStreamTo)
        CustomTransferFunc(float, ReadStreamTo)
        CustomTransferFunc(bool, ReadStreamTo)
        CustomTransferFunc(std::string, TransferString)
    };
}