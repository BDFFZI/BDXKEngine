#include "Texture2D.h"

namespace BDXKEngine
{
    ObjectPtr<Texture2D> Texture2D::Create(const int widthf, const int heightf)
    {
        const auto texture2d = new Texture2D{};
        texture2d->width = widthf;
        texture2d->height = heightf;
        return Instantiate<Texture2D>(texture2d);
    }

    ObjectPtr<Texture2D> Texture2D::Create(Color color)
    {
        const auto texture2d = new Texture2D{};
        texture2d->width = 1;
        texture2d->height = 1;
        texture2d->pixels = std::unique_ptr<Color[]>(new Color[]{color});
        return Instantiate<Texture2D>(texture2d);
    }
    void Texture2D::SetRenderTarget() const
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
        context->OMSetRenderTargets(1, &renderTextureRTV.p, depthTextureDSV.p);
    }

    void Texture2D::Awake()
    {
        Texture::Awake();

        if (pixels == nullptr)
            pixels = std::unique_ptr<Color[]>(new Color[static_cast<size_t>(width * height)]);

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

        HRESULT result = GL::GetDevice()->CreateTexture2D(&renderTextureDesc, &resourceData, &renderTexture.p);
        if (FAILED(SUCCEEDED(result)))throw std::exception("创建颜色纹理失败");

        //创建渲染目标视图，使其可作为画布承载渲染结果
        D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc{};
        renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
        result = GL::GetDevice()->CreateRenderTargetView(renderTexture, &renderTargetViewDesc, &renderTextureRTV.p);
        if (FAILED(SUCCEEDED(result)))throw std::exception("创建颜色纹理视图失败");

        //创建渲染纹理着色器资源视图，使其可作为贴图使用
        result = GL::GetDevice()->CreateShaderResourceView(renderTexture, nullptr, &colorTextureSRV.p);
        if (FAILED(SUCCEEDED(result)))throw std::exception("创建颜色纹理着色器资源视图失败");

        //创建深度纹理
        CreateDepthStencil(width, height, &depthTexture.p, &depthTextureDSV.p);

        //创建采样器
        CreateSamplerState(&samplerState.p);
    }
}
