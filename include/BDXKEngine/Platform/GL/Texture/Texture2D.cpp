#include "Texture2D.h"

namespace BDXKEngine
{
    ObjectPtr<Texture2D> Texture2D::Create(Color color)
    {
        const auto texture2d = new Texture2D{};
        texture2d->width = 1;
        texture2d->height = 1;
        texture2d->format = TextureFormat::B8G8R8A8_UNORM;

        const unsigned char* data = new unsigned char[]
        {
            static_cast<unsigned char>(color.b * 255),
            static_cast<unsigned char>(color.g * 255),
            static_cast<unsigned char>(color.r * 255),
            static_cast<unsigned char>(color.a * 255)
        };
        texture2d->pixels.resize(GetPixelSize(TextureFormat::B8G8R8A8_UNORM));
        memcpy_s(texture2d->pixels.data(), texture2d->pixels.size(), data, texture2d->pixels.size());
        delete[] data;

        return Instantiate<Texture2D>(texture2d);
    }
    ObjectPtr<Texture2D> Texture2D::Create(const int widthf, const int heightf, TextureFormat textureFormat)
    {
        const auto texture2d = new Texture2D{};
        texture2d->width = widthf;
        texture2d->height = heightf;
        texture2d->format = textureFormat;
        texture2d->pixels.resize(static_cast<size_t>(widthf) * heightf * GetPixelSize(textureFormat));
        return Instantiate<Texture2D>(texture2d);
    }

    ObjectPtr<Texture2D> Texture2D::Create(const int widthf, const int heightf, TextureFormat textureFormat, const char* pixels)
    {
        const auto texture2d = new Texture2D{};
        texture2d->width = widthf;
        texture2d->height = heightf;
        texture2d->format = textureFormat;
        texture2d->pixels.resize(static_cast<size_t>(widthf) * heightf * GetPixelSize(textureFormat));
        memcpy_s(texture2d->pixels.data(), texture2d->pixels.size(), pixels, texture2d->pixels.size());
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

        renderTarget = this;
    }

    void Texture2D::Transfer(Transferer& transferer)
    {
        Texture::Transfer(transferer);

        TransferFieldInfo(pixels);
    }
    void Texture2D::Awake()
    {
        Texture::Awake();

        //创建渲染纹理
        D3D11_TEXTURE2D_DESC renderTextureDesc = {};
        renderTextureDesc.Format = static_cast<DXGI_FORMAT>(format);
        renderTextureDesc.Width = width;
        renderTextureDesc.Height = height;
        renderTextureDesc.ArraySize = 1;
        renderTextureDesc.MipLevels = 1;
        renderTextureDesc.SampleDesc = {1, 0};
        renderTextureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;

        D3D11_SUBRESOURCE_DATA resourceData = {};
        resourceData.pSysMem = pixels.data();
        resourceData.SysMemPitch = width * GetPixelSize(format);

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
