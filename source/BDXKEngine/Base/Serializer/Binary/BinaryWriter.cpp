#include "BinaryWriter.h"

namespace BDXKEngine
{
    bool BinaryWriter::IsImporter()
    {
        return false;
    }
    void BinaryWriter::Reset(std::string& data)
    {
        data = stream.str();
        stream.str("");
    }
    void BinaryWriter::WriteStreamFrom(const char* value, int size)
    {
        stream.write(value, size);
    }

    void BinaryWriter::TransferString(const std::string& value)
    {
        int size = static_cast<int>(value.size());
        const char* data = value.data();

        WriteStreamFrom(size);
        WriteStreamFrom(data, size);
    }
    void BinaryWriter::TransferBytes(const std::vector<char>& value)
    {
        int size = static_cast<int>(value.size());

        WriteStreamFrom(size);
        WriteStreamFrom(value.data(), size);
    }
}
