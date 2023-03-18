#include "BinaryReader.h"

namespace BDXKEngine
{
    bool BinaryReader::IsImporter()
    {
        return true;
    }
    void BinaryReader::Reset(std::string& data)
    {
        stream.str(data);
    }

    void BinaryReader::ReadStreamTo(char* value, int size)
    {
        stream.read(value, size);
    }

    void BinaryReader::TransferString(std::string& value)
    {
        int size = 0;
        ReadStreamTo(size);
        char* data = new char[size];
        ReadStreamTo(data, size);

        value = std::string(data, size);

        delete[] data;
    }
    void BinaryReader::TransferBytes(std::vector<char>& value)
    {
        int size = 0;
        ReadStreamTo(size);
        value.resize(size);
        ReadStreamTo(value.data(), size);
    }
}
