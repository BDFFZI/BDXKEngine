#pragma once
#include "BDXKEngine/Base/Reflection/Transferer.h"

namespace BDXKEngine
{
    class Importer : public Transferer
    {
    public:
        virtual void Import(std::string& data) = 0;
    };
}
