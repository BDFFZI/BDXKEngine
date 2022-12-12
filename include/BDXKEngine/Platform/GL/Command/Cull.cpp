#include "Cull.h"

namespace BDXKEngine
{
    Cull Cull::Default = {};

    Cull::Cull() = default;
    Cull::Cull(State state): Cull()
    {
        this->state = state;
    }

    void Cull::UpdateGL()
    {
        rasterizerState = nullptr;
        D3D11_RASTERIZER_DESC description;
        description.FillMode = D3D11_FILL_SOLID;
        description.CullMode = static_cast<D3D11_CULL_MODE>(state);
        description.FrontCounterClockwise = false;
        description.DepthBias = 0;
        description.DepthBiasClamp = 0;
        description.SlopeScaledDepthBias = 0;
        description.DepthClipEnable = true;
        description.ScissorEnable = false;
        description.MultisampleEnable = false;
        description.AntialiasedLineEnable = false;
        GL::GetDevice()->CreateRasterizerState(&description, &rasterizerState.p);
    }
    void Cull::UploadRP() const
    {
        GL::GetDeviceContext()->RSSetState(rasterizerState);
    }
    void Cull::Transfer(Transferer& transferer)
    {
        TransferFieldInfoOf(state, int);
    }
}
