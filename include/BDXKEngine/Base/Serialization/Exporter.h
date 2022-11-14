#pragma once
#include "BDXKEngine/Base/Reflection/Transferer.h"

namespace BDXKEngine
{
    class Exporter : public Transferer
    {
    public:
        virtual void Export(std::string& data) = 0;
    };
}
