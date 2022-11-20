#pragma once
#include "BDXKEngine/Base/Object/Core/Object.h"
#include "../GL.h"

namespace BDXKEngine
{
    class TextureEditor;

    class Texture : public Object
    {
    public:
        int GetWidth() const;
        int GetHeight() const;
        Vector2 GetSize() const;

        void SetPass(unsigned int startSlot) const; // 设置当前渲染管线中的着色器资源,如纹理之类的
    protected:
        static void CreateDepthStencil(unsigned int width, unsigned int height, ID3D11Texture2D** renderTexture,
                                       ID3D11DepthStencilView** depthStencilView);
        static void CreateSamplerState(ID3D11SamplerState** samplerState);

        int width = 0;
        int height = 0;

        CComPtr<ID3D11Texture2D> renderTexture = nullptr;
        CComPtr<ID3D11Texture2D> depthTexture = nullptr;
        CComPtr<ID3D11DepthStencilView> depthTextureDSV = nullptr;

        CComPtr<ID3D11SamplerState> samplerState;
        CComPtr<ID3D11ShaderResourceView> colorTextureSRV = nullptr;

        void Transfer(Transferer& transferer) override;
    };
}
