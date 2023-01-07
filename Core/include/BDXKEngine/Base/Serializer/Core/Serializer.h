#pragma once
#include "IOTransferer.h"
#include "BDXKEngine/Base/Reflection/Reflective.h"

namespace BDXKEngine
{
    std::string ReadFile(const std::string& path);
    void WriteFile(const std::string& path, const std::string& data);

    class Serializer
    {
    public:
        Serializer(IOTransferer& importer, IOTransferer& exporter);
        std::string Serialize(Reflective* input) const;
        Reflective* Deserialize(std::string input) const;
        Reflective* Clone(Reflective* input) const;
    private:
        IOTransferer& importer;
        IOTransferer& exporter;
    };

    template <typename TImporter, typename TExporter>
    class Serializer1 : public Serializer
    {
    public:
        Serializer1(): Serializer(importer, exporter)
        {
        }
    private:
        TImporter importer;
        TExporter exporter;
    };
}
