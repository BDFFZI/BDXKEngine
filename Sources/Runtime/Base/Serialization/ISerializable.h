#pragma once
#include "Transferrer.h"

namespace BDXKEngine
{
#define nameof(Field) L""#Field""

    struct ISerializable
    {
        virtual void Transfer(Transferrer& transferrer) = 0;

        virtual ~ISerializable() = default;
    };
}
