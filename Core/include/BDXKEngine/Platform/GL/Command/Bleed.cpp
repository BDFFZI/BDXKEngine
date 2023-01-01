#include "Blend.h"

namespace BDXKEngine
{
    Blend Blend::Opaque = {};
    Blend Blend::Multiply = {true, Factor::SrcAlpha, Factor::OneMinusSrcAlpha, Operation::Add};
    Blend Blend::Additive = {true, Factor::One, Factor::One, Operation::Add};

    Blend::Blend() = default;
    Blend::Blend(bool state, Factor sourceFactor, Factor destinationFactor, Operation operation): Blend()
    {
        this->state = state;
        this->sourceFactor = sourceFactor;
        this->destinationFactor = destinationFactor;
        this->operation = operation;
    }

    void Blend::UpdateGL()
    {
        blendState = nullptr;
        D3D11_RENDER_TARGET_BLEND_DESC renderTargetBlendDescription;
        renderTargetBlendDescription.BlendEnable = state;
        renderTargetBlendDescription.SrcBlend = static_cast<D3D11_BLEND>(sourceFactor);
        renderTargetBlendDescription.DestBlend = static_cast<D3D11_BLEND>(destinationFactor);
        renderTargetBlendDescription.BlendOp = static_cast<D3D11_BLEND_OP>(operation);
        renderTargetBlendDescription.SrcBlendAlpha = D3D11_BLEND_ONE;
        renderTargetBlendDescription.DestBlendAlpha = D3D11_BLEND_ONE;
        renderTargetBlendDescription.BlendOpAlpha = D3D11_BLEND_OP_MAX;
        renderTargetBlendDescription.RenderTargetWriteMask = 15;
        D3D11_BLEND_DESC blendDescription{};
        blendDescription.RenderTarget[0] = renderTargetBlendDescription;
        const HRESULT result = GL::GetDevice()->CreateBlendState(&blendDescription, &blendState.p);
        if (FAILED(result))throw std::exception("创建渲染混合参数失败");
    }
    void Blend::UploadRP() const
    {
        GL::GetDeviceContext()->OMSetBlendState(blendState, nullptr, 0xffffffff);
    }
    void Blend::Transfer(Transferer& transferer)
    {
        TransferFieldInfo(state);
        TransferFieldInfoOf(sourceFactor, int);
        TransferFieldInfoOf(destinationFactor, int);
        TransferFieldInfoOf(operation, int);
    }
}
