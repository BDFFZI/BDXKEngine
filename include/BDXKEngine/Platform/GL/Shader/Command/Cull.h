#pragma once
#include <d3d11.h>
#include "BDXKEngine/Base/Reflection/Transferer.h"

namespace BDXKEngine
{
    struct Cull : Transferable
    {
        enum class State
        {
            Off = D3D11_CULL_NONE,
            Back = D3D11_CULL_BACK,
            Front = D3D11_CULL_FRONT,
        };

        static Cull Default;

        State state = State::Back;

        Cull();
        Cull(State state);
        void Transfer(Transferer& transferer) override;
    };
}
