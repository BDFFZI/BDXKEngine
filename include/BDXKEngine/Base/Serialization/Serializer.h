#pragma once
#include "Exporter.h"
#include "Importer.h"
#include "../Reflection/Reflective.h"

namespace BDXKEngine
{
    class Serializer
    {
    public:
        Serializer(Importer& importer, Exporter& exporter);

        virtual ~Serializer() = default;
        virtual std::string Serialize(Reflective* input);
        virtual Reflective* Deserialize(std::string input);
        virtual Reflective* Clone(Reflective* input);
    private:
        Importer& importer;
        Exporter& exporter;
    };
}
