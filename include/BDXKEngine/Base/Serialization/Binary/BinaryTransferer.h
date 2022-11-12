#pragma once
#include "BDXKEngine/Base/Reflection/Transferer.h"

namespace BDXKEngine
{
    class BinaryTransferer : public Transferer
    {
    protected:
        BinaryTransferer(const std::shared_ptr<std::iostream>& stream, bool isReadOnly);

        void WriteStreamFrom(const char* value, int size) const;
        void ReadStreamTo(char* value, int size) const;

        template <typename TValue>
        void WriteStreamFrom(TValue& value)
        {
            WriteStreamFrom(reinterpret_cast<char*>(&value), sizeof(TValue));
        }
        template <typename TValue>
        void ReadStreamTo(TValue& value)
        {
            ReadStreamTo(reinterpret_cast<char*>(&value), sizeof(TValue));
        }
    private:
        std::shared_ptr<std::iostream> stream;
    };
}
