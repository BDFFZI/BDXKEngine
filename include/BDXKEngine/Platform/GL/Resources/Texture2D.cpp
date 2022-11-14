#include "Texture2D.h"
#include "BDXKEngine/Platform/GL/GL.h"

namespace BDXKEngine
{
    ObjectPtr<Texture2D> Texture2D::Create(const int width, const int height)
    {
        Texture2D texture2d = {};
        texture2d.width = width;
        texture2d.height = height;
        texture2d.pixels = std::unique_ptr<Color[]>(new Color[width * height]);

        return Object::Instantiate<Texture2D>(&texture2d);
    }

    ObjectPtr<Texture2D> Texture2D::Create(Color color)
    {
        Texture2D texture2d = {};
        texture2d.width = 1;
        texture2d.height = 1;
        texture2d.pixels = std::unique_ptr<Color[]>(new Color[]{color});

        return Object::Instantiate<Texture2D>(&texture2d);
    }

    void Texture2D::Transfer(transferer& transferer)
    {
        Texture::Transfer(transferer);

        if (transferer.GetTransferDirection() == TransferDirection::Input)pixels = std::unique_ptr<Color[]>(new Color[width * height]);
        transferer.TransferField(nameof(pixels), reinterpret_cast<char*>(pixels.get()), width * height * static_cast<int>(sizeof(Color)));
    }
    void Texture2D::MarkAwake()
    {
        HRESULT result = {};

        //创建渲染纹理
        D3D11_TEXTURE2D_DESC renderTextureDesc = {};
        renderTextureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
        renderTextureDesc.Width = width;
        renderTextureDesc.Height = height;
        renderTextureDesc.ArraySize = 1;
        renderTextureDesc.MipLevels = 1;
        renderTextureDesc.SampleDesc = {1, 0};
        renderTextureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;

        D3D11_SUBRESOURCE_DATA resourceData = {};
        resourceData.pSysMem = pixels.get();
        resourceData.SysMemPitch = width * sizeof(Color);

        result = device->CreateTexture2D(&renderTextureDesc, &resourceData, &colorTexture.p);
        assert(SUCCEEDED(result));

        //创建渲染目标视图，使其可作为画布承载渲染结果
        D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc{};
        renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
        result = device->CreateRenderTargetView(colorTexture, &renderTargetViewDesc, &renderTextureRTV.p);
        assert(SUCCEEDED(result));

        //创建渲染纹理着色器资源视图，使其可作为贴图使用
        result = device->CreateShaderResourceView(colorTexture, nullptr, &colorTextureSRV.p);
        assert(SUCCEEDED(result));

        //创建深度纹理
        CreateDepthStencil(width, height, &depthTexture.p, &depthTextureDSV.p);

        //创建采样器
        CreateSamplerState(&samplerState.p);
    }
}
