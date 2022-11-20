#include "BinaryImporter.h"

namespace BDXKEngine
{
    void BinaryImporter::Reset(std::string& data)
    {
        stream.str(data);
    }

    void BinaryImporter::ReadStreamTo(char* value, int size)
    {
        stream.read(value, size);
    }

    void BinaryImporter::TransferString(std::string& value)
    {
        int size = 0;
        ReadStreamTo(size);
        char* data = new char[size];
        ReadStreamTo(data, size);

        value = std::string(data, size);

        delete[] data;
    }
    void BinaryImporter::TransferBytes(std::vector<char>& value)
    {
        int size = 0;
        ReadStreamTo(size);
        value.resize(size);
        ReadStreamTo(value.data(), size);
    }
}
