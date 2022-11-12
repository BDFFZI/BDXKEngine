#include "BinaryTransferer.h"
#include <iostream>

namespace BDXKEngine
{
    BinaryTransferer::BinaryTransferer(const std::shared_ptr<std::iostream>& stream, bool isReadOnly): Transferer(isReadOnly),
        stream(stream)
    {
    }

    void BinaryTransferer::WriteStreamFrom(const char* value, int size) const
    {
        stream->write(value, size);
    }

    void BinaryTransferer::ReadStreamTo(char* value, int size) const
    {
        stream->read(value, size);
    }
}
