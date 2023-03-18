#pragma once
#include "Transferer.h"

namespace BDXKEngine
{
    class Reflective
    {
    public:
        static Type GetType(Transferer& transferer);

        Reflective() = default;
        Reflective(const Reflective& reflective) = default;
        virtual ~Reflective() = default;

        Type GetType() const;
        virtual void Transfer(Transferer& transferer);
    };
}
