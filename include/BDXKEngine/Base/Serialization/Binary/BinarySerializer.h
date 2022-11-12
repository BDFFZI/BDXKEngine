#pragma once
#include "../Serializer.h"

namespace BDXKEngine
{
    class BinarySerializer : public Serializer
    {
    public:
        std::string Serialize(Reflective* input) override;
        Reflective* Deserialize(std::string input) override;
        Reflective* Clone(Reflective* input) override;
    };
}
