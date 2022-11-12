#pragma once
#include "../Reflection/Reflective.h"

namespace BDXKEngine
{
    class Serializer
    {
    public:
        virtual ~Serializer() = default;
        virtual std::string Serialize(Reflective* input) = 0;
        virtual Reflective* Deserialize(std::string input) = 0;
        virtual Reflective* Clone(Reflective* input) = 0;
    };
}
