#pragma once
#include "Transferer.h"

namespace BDXKEngine
{
    class Reflective : public Transferable
    {
    public:
        static Type GetType(Transferer& transferer);

        Type GetType() const;

        void Transfer(Transferer& transferer) override;

        Reflective() = default;
        Reflective(const Reflective& reflective) = default;
    };
}
