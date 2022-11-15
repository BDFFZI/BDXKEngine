#pragma once
#include "../Reflection/Reflective.h"

namespace BDXKEngine
{
    class Serializer
    {
    public:
        Serializer(Transferer& importer, Transferer& exporter);

        virtual ~Serializer() = default;
        virtual std::string Serialize(Reflective* input);
        virtual Reflective* Deserialize(std::string input);
        virtual Reflective* Clone(Reflective* input);
    private:
        Transferer& importer;
        Transferer& exporter;
    };
}
