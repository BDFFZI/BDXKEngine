#pragma once
#include "Core/Runtime/Platform/GL/Core/GL.h"
#include "Core/Runtime/Base/Reflection/Transferer.h"

namespace BDXKEngine
{
    struct Cull
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

        void UpdateGL();
        void UploadRP() const;
        void Transfer(Transferer& transferer);
    private:
        CComPtr<ID3D11RasterizerState> rasterizerState;
    };
}
