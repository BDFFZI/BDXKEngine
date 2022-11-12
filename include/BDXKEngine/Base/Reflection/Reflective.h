#pragma once
#include "Transferer.h"

namespace BDXKEngine
{
    class Reflective
    {
    public:
        static Type GetTypeID(Transferer& transferer);

        Type GetTypeID() const;

        virtual void Transfer(Transferer& transferer);

        Reflective() = default;
        Reflective(const Reflective& reflective) = default;
        virtual ~Reflective() = default;
    };
}
