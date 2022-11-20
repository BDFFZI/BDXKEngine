#include "BinaryExporter.h"

namespace BDXKEngine
{
    void BinaryExporter::Reset(std::string& data)
    {
        data = stream.str();
        stream.str("");
    }

    void BinaryExporter::WriteStreamFrom(const char* value, int size)
    {
        stream.write(value, size);
    }

    void BinaryExporter::TransferString(const std::string& value)
    {
        int size = static_cast<int>(value.size());
        const char* data = value.data();

        WriteStreamFrom(size);
        WriteStreamFrom(data, size);
    }
    void BinaryExporter::TransferBytes(const std::vector<char>& value)
    {
        int size = static_cast<int>(value.size());
        const char* data = value.data();

        WriteStreamFrom(size);
        WriteStreamFrom(data, size);
    }
}
