#pragma once
#include <string>
#include <typeinfo>
#include "Transferer.h"

namespace BDXKEngine
{
    typedef std::string Type;

    class Serialization
    {
    public:
        template <typename TObject>
        static Type GetTypeID()
        {
            return typeid(TObject).name();
        }

        Type GetTypeID();
        virtual void Transfer(Transferer& transferer);

        virtual ~Serialization() = default;
    };
}

// struct SerializeTester : Serializable
// {
//     void Transfer(transferer& transferer) override
//     {
//         transferer.TransferField("stringValue", stringValue);
//         transferer.TransferField("floatValue", floatValue);
//         transferer.TransferFieldOf<Vector4>("colorValue", colorValue);
//         transferer.TransferField("boolValue", boolValue);
//     }
//
//     std::string stringValue = "BDXK引擎";
//     float floatValue;
//     Color colorValue;
//     bool boolValue;
// };
