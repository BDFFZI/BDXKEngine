#pragma once
#include "IOTransferer.h"
#include "BDXKEngine/Base/Reflection/Reflective.h"

namespace BDXKEngine
{
    class Serializer
    {
    public:
        Serializer(IOTransferer& importer, IOTransferer& exporter);

        virtual ~Serializer() = default;
        virtual std::string Serialize(Reflective* input);
        virtual Reflective* Deserialize(std::string input);
        virtual Reflective* Clone(Reflective* input);
    private:
        IOTransferer& importer;
        IOTransferer& exporter;
    };
}
