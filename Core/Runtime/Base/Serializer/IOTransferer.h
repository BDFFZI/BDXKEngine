#pragma once
#include "Core/Runtime/Base/Reflection/Transferer.h"

namespace BDXKEngine
{
    class IOTransferer : public Transferer
    {
    public:
        virtual bool IsImporter() = 0;
        virtual void Reset(std::string& data) = 0;
    };
}
