#pragma once
#include "BDXKEngine/Platform/GL/GL.h"

namespace BDXKEngine
{
    class TextureEditor;

    class Texture : public Object, protected GL
    {
        friend TextureEditor;
        friend GL;
    public:
        int GetWidth() const;
        int GetHeight() const;
        Vector2 GetSize() const;
    protected:
        int width;
        int height;

        CComPtr<ID3D11Texture2D> colorTexture = nullptr;
        CComPtr<ID3D11Texture2D> depthTexture = nullptr;
        CComPtr<ID3D11DepthStencilView> depthTextureDSV = nullptr;

        CComPtr<ID3D11SamplerState> samplerState;
        CComPtr<ID3D11ShaderResourceView> colorTextureSRV = nullptr;

        static void CreateDepthStencil(unsigned int width, unsigned int height, ID3D11Texture2D** renderTexture,
                                       ID3D11DepthStencilView** depthStencilView);
        static void CreateSamplerState(ID3D11SamplerState** samplerState);

        CComPtr<ID3D11ShaderResourceView> GetResourceView();
        CComPtr<ID3D11SamplerState> GetSamplerState();

        void Transfer(Transferrer& transferrer) override;
    };

    class TextureEditor
    {
    public:
        static CComPtr<ID3D11ShaderResourceView> GetShaderResourceView(ObjectPtr<Texture> texture);
    };
}
