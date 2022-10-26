#include "TextureCube.h"
using namespace BDXKEngine;

namespace BDXKEngine
{
    ObjectPtr<TextureCube> TextureCube::Create(const int width, const int height)
    {
        TextureCube textureCube = {};
        textureCube.width = width;
        textureCube.height = height;

        return Instantiate<TextureCube>(&textureCube);
    }

    void TextureCube::PreAwake()
    {
        HRESULT result = {};

        //创建渲染纹理
        D3D11_TEXTURE2D_DESC renderTextureDesc = {};
        renderTextureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
        renderTextureDesc.Width = width;
        renderTextureDesc.Height = height;
        renderTextureDesc.ArraySize = 6;
        renderTextureDesc.MipLevels = 1;
        renderTextureDesc.SampleDesc = {1, 0};
        renderTextureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
        renderTextureDesc.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE;

        result = device->CreateTexture2D(&renderTextureDesc, nullptr, &colorTexture.p);
        assert(SUCCEEDED(result));

        //创建渲染纹理资源视图
        D3D11_SHADER_RESOURCE_VIEW_DESC SMViewDesc = {};
        SMViewDesc.Format = renderTextureDesc.Format;
        SMViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
        SMViewDesc.TextureCube = {0, 1};
        result = device->CreateShaderResourceView(colorTexture, &SMViewDesc, &colorTextureSRV.p);
        assert(SUCCEEDED(result));

        //创建渲染纹理渲染目标视图
        for (unsigned int index = 0; index < 6; index++)
        {
            D3D11_RENDER_TARGET_VIEW_DESC renderTargetDesc = {};
            renderTargetDesc.Format = renderTextureDesc.Format;
            renderTargetDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DARRAY;
            renderTargetDesc.Texture2DArray = {0, index, 1};

            CComPtr<ID3D11RenderTargetView> renderTargetView;
            result = device->CreateRenderTargetView(colorTexture, &renderTargetDesc, &renderTargetView.p);
            assert(SUCCEEDED(result));
            renderTextureRTV.push_back(renderTargetView);
        }

        //创建深度纹理
        CreateDepthStencil(width, height, &depthTexture.p, &depthTextureDSV.p);

        //创建采样器
        CreateSamplerState(&samplerState.p);
    }
}
