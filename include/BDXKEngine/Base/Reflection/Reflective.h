#pragma once
#include "Transferer.h"

namespace BDXKEngine
{
    class Reflective : public Transferable
    {
    public:
        static Type GetTypeID(Transferer& transferer);

        Type GetTypeID() const;

        void Transfer(Transferer& transferer) override;

        Reflective() = default;
        Reflective(const Reflective& reflective) = default;
    };
}
