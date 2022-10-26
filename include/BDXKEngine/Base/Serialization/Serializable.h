#pragma once
#include "Transferrer.h"

namespace BDXKEngine
{
#define nameof(Field) ""#Field""

    class Serializable
    {
    public:
        virtual void Transfer(Transferrer& transferrer) = 0;

        virtual ~Serializable() = default;
    };
}

// struct SerializeTester : Serializable
// {
//     void Transfer(Transferrer& transferrer) override
//     {
//         transferrer.TransferField("stringValue", stringValue);
//         transferrer.TransferField("floatValue", floatValue);
//         transferrer.TransferFieldOf<Vector4>("colorValue", colorValue);
//         transferrer.TransferField("boolValue", boolValue);
//     }
//
//     std::string stringValue = "BDXK引擎";
//     float floatValue;
//     Color colorValue;
//     bool boolValue;
// };
