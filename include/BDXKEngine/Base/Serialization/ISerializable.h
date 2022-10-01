#pragma once
#include "Transferrer.h"

namespace BDXKEngine
{
#define nameof(Field) ""#Field""

    class ISerializable
    {
    public:
        virtual void Transfer(Transferrer& transferrer) = 0;

        virtual ~ISerializable() = default;
    };
}
