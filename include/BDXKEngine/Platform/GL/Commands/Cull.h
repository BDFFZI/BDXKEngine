#pragma once

#include "BDXKEngine/Base/Reflection/Transferer.h"

namespace BDXKEngine
{
    struct Cull : Transferable
    {
        enum class State
        {
            Back,
            Front,
            Off,
        };

        State state = State::Back;

        void Transfer(Transferer& transferer) override;
    };
}
