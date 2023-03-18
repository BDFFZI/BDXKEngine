#include "TextureCube.h"
using namespace BDXKEngine;

namespace BDXKEngine
{
    ObjectPtr<TextureCube> TextureCube::Create(const int widthf, const int heightf, TextureFormat textureFormat)
    {
        ObjectPtr textureCube = new TextureCube{};
        textureCube->width = widthf;
        textureCube->height = heightf;
        textureCube->format = textureFormat;
        Instantiate(textureCube);

        return textureCube;
    }
    void TextureCube::SetRenderTarget(int index) const
    {
        const auto context = GL::GetDeviceContext();

        //调整视口数据至与新纹理大小一致
        D3D11_VIEWPORT viewport = {};
        viewport.Height = static_cast<float>(height);
        viewport.Width = static_cast<float>(width);
        viewport.MinDepth = 0;
        viewport.MaxDepth = 1;
        context->RSSetViewports(1, &viewport);

        //设置渲染目标
        context->OMSetRenderTargets(1, &renderTextureRTV[index].p, depthTextureDSV);

        renderTarget = this;
    }

    void TextureCube::Awake()
    {
        Texture::Awake();

        HRESULT result = {};

        //创建渲染纹理
        D3D11_TEXTURE2D_DESC renderTextureDesc = {};
        renderTextureDesc.Format = static_cast<DXGI_FORMAT>(format);
        renderTextureDesc.Width = width;
        renderTextureDesc.Height = height;
        renderTextureDesc.ArraySize = 6;
        renderTextureDesc.MipLevels = 1;
        renderTextureDesc.SampleDesc = {1, 0};
        renderTextureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
        renderTextureDesc.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE;

        result = GL::GetDevice()->CreateTexture2D(&renderTextureDesc, nullptr, &renderTexture.p);
        if (FAILED(SUCCEEDED(result)))throw std::exception("创建颜色纹理失败");

        //创建渲染纹理资源视图
        D3D11_SHADER_RESOURCE_VIEW_DESC SMViewDesc = {};
        SMViewDesc.Format = renderTextureDesc.Format;
        SMViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
        SMViewDesc.TextureCube = {0, 1};
        result = GL::GetDevice()->CreateShaderResourceView(renderTexture, &SMViewDesc, &colorTextureSRV.p);
        if (FAILED(SUCCEEDED(result)))throw std::exception("创建颜色纹理着色器资源视图失败");

        //创建渲染纹理渲染目标视图
        for (unsigned int index = 0; index < 6; index++)
        {
            D3D11_RENDER_TARGET_VIEW_DESC renderTargetDesc = {};
            renderTargetDesc.Format = renderTextureDesc.Format;
            renderTargetDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DARRAY;
            renderTargetDesc.Texture2DArray = {0, index, 1};

            CComPtr<ID3D11RenderTargetView> renderTargetView;
            result = GL::GetDevice()->CreateRenderTargetView(renderTexture, &renderTargetDesc, &renderTargetView.p);
            if (FAILED(SUCCEEDED(result)))throw std::exception("创建颜色纹理视图失败");
            renderTextureRTV.push_back(renderTargetView);
        }

        //创建深度纹理
        CreateDepthStencil(width, height, &depthTexture.p, &depthTextureDSV.p);

        //创建采样器
        CreateSamplerState(&samplerState.p);
    }
}
