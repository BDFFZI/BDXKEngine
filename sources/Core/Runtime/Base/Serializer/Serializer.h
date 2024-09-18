#pragma once
#include <filesystem>

#include "IOTransferer.h"
#include "Core/Runtime/Base/Reflection/Reflective.h"

namespace BDXKEngine
{
    std::string ReadFile(const std::filesystem::path& path);
    void WriteFile(const std::filesystem::path& path, const std::string& data);

    class Serializer
    {
    public:
        Serializer(IOTransferer& reader, IOTransferer& writer);
        std::string Serialize(Reflective* input) const;
        Reflective* Deserialize(std::string input) const;
        Reflective* Clone(Reflective* input) const;

    private:
        IOTransferer& reader;
        IOTransferer& writer;
    };

    template <typename TReader, typename TWriter>
    class SerializerOf : public Serializer
    {
    public:
        SerializerOf(): Serializer(reader, writer)
        {
        }

    private:
        TReader reader;
        TWriter writer;
    };
}
