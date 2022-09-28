#pragma once
#include "Platform/GL/GL.h"

namespace BDXKEngine
{
    class Texture : public Object, protected GL
    {
        friend GL;
    public:
        unsigned int GetWidth() const;
        unsigned int GetHeight() const;
    protected:
        int width;
        int height;

        CComPtr<ID3D11SamplerState> samplerState;
        D3D11_TEXTURE2D_DESC renderTextureDesc;
        CComPtr<ID3D11Texture2D> renderTexture = nullptr;
        CComPtr<ID3D11ShaderResourceView> renderTextureSRV = nullptr;
        CComPtr<ID3D11Texture2D> depthTexture = nullptr;
        CComPtr<ID3D11DepthStencilView> depthTextureDSV = nullptr;

        static void CreateDepthStencil(unsigned int width, unsigned int height, ID3D11Texture2D** renderTexture,
                                       ID3D11DepthStencilView** depthStencilView);
        static void CreateSamplerState(ID3D11SamplerState** samplerState);

        CComPtr<ID3D11ShaderResourceView> GetResourceView();
        CComPtr<ID3D11SamplerState> GetSamplerState();

        void Transfer(Transferrer& transferrer) override;
    };
}
