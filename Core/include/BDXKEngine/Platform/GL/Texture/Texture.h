#pragma once
#include "TextureFormat.h"
#include "BDXKEngine/Base/Object/Object.h"
#include "BDXKEngine/Base/Data/Mathematics/Vector/Vector2.h"
#include "../Core/GL.h"

namespace BDXKEngine
{
    class Texture : public Object
    {
    public:
        static ObjectPtr<Texture> GetRenderTarget();
        static void SetRenderTargetDefault();
        static void ResetDefaultRenderTarget();

        static void UploadRPNull(unsigned startSlot);

        int GetWidth() const;
        int GetHeight() const;
        Vector2 GetSize() const;
        CComPtr<ID3D11ShaderResourceView> GetShaderResourceView();

        void UploadRP(unsigned int startSlot) const; // 设置当前渲染管线中的着色器资源,如纹理之类的
    protected:
        static ObjectPtr<Texture> renderTarget;
        static D3D11_TEXTURE2D_DESC defaultRenderTargetDescription;
        static CComPtr<ID3D11RenderTargetView> defaultRenderTargetView;
        static CComPtr<ID3D11DepthStencilView> defaultDepthStencilView;

        static void CreateDepthStencil(unsigned int width, unsigned int height, ID3D11Texture2D** renderTexture,
                                       ID3D11DepthStencilView** depthStencilView);
        static void CreateSamplerState(ID3D11SamplerState** samplerState);

        int width = 0;
        int height = 0;
        TextureFormat format = TextureFormat::None;

        CComPtr<ID3D11Texture2D> renderTexture = nullptr;
        CComPtr<ID3D11Texture2D> depthTexture = nullptr;
        CComPtr<ID3D11DepthStencilView> depthTextureDSV = nullptr;

        CComPtr<ID3D11SamplerState> samplerState;
        CComPtr<ID3D11ShaderResourceView> colorTextureSRV = nullptr;

        void Transfer(Transferer& transferer) override;
    };
}
