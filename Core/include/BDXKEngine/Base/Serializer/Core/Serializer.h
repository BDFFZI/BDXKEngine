#pragma once
#include "IOTransferer.h"
#include "BDXKEngine/Base/Reflection/Reflective.h"

namespace BDXKEngine
{
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
}
