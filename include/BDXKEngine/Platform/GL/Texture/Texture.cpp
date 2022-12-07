#include "Texture.h"

namespace BDXKEngine
{
    ObjectPtr<Texture> Texture::renderTarget;
    D3D11_TEXTURE2D_DESC Texture::defaultRenderTargetDescription;
    CComPtr<ID3D11RenderTargetView> Texture::defaultRenderTargetView;
    CComPtr<ID3D11DepthStencilView> Texture::defaultDepthStencilView;


    ObjectPtr<Texture> Texture::GetRenderTarget()
    {
        return renderTarget;
    }
    void Texture::SetRenderTargetDefault()
    {
        if (defaultRenderTargetDescription.Height * defaultRenderTargetDescription.Width == 0)
            throw std::exception("默认渲染视图尚未创建");

        const auto context = GL::GetDeviceContext();
        //调整视口数据至与新纹理大小一致
        D3D11_VIEWPORT viewport = {};
        viewport.Height = static_cast<float>(defaultRenderTargetDescription.Height);
        viewport.Width = static_cast<float>(defaultRenderTargetDescription.Width);
        viewport.MinDepth = 0;
        viewport.MaxDepth = 1;
        context->RSSetViewports(1, &viewport);

        //设置渲染目标
        context->OMSetRenderTargets(1, &defaultRenderTargetView.p, defaultDepthStencilView);

        renderTarget = nullptr;
    }
    void Texture::SetPassNull(unsigned startSlot)
    {
        const auto context = GL::GetDeviceContext();
        ID3D11ShaderResourceView* resourceView = nullptr;
        ID3D11SamplerState* samplerState = nullptr;
        context->PSSetShaderResources(startSlot, 1, &resourceView);
        context->PSSetSamplers(startSlot, 1, &samplerState);
    }
    void Texture::ResetDefaultRenderTarget()
    {
        const auto swapChain = GL::GetSwapChain();
        const auto device = GL::GetDevice();

        //需要重置纹理大小，但视图一直占用着纹理而导致没法重置，所以要先释放视图
        defaultRenderTargetView = nullptr;
        defaultDepthStencilView = nullptr;

        //重置染目标纹理大小
        swapChain->ResizeBuffers(2, 0, 0, DXGI_FORMAT_B8G8R8A8_UNORM, 0);
        //获取新的渲染目标纹理及其参数
        CComPtr<ID3D11Texture2D> targetTexture = nullptr;
        swapChain->GetBuffer(0, IID_PPV_ARGS(&targetTexture.p));
        targetTexture->GetDesc(&defaultRenderTargetDescription);
        //以该参数创建用于深度模板测试的纹理
        CComPtr<ID3D11Texture2D> depthStencilTexture = nullptr;
        D3D11_TEXTURE2D_DESC depthStencilTextureDescription = {};
        depthStencilTextureDescription.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
        depthStencilTextureDescription.BindFlags = D3D11_BIND_DEPTH_STENCIL;
        depthStencilTextureDescription.Width = defaultRenderTargetDescription.Width;
        depthStencilTextureDescription.Height = defaultRenderTargetDescription.Height;
        depthStencilTextureDescription.MipLevels = 1;
        depthStencilTextureDescription.ArraySize = 1;
        depthStencilTextureDescription.SampleDesc = {1, 0};
        HRESULT result = device->CreateTexture2D(&depthStencilTextureDescription, nullptr, &depthStencilTexture.p);
        if (FAILED(result))throw std::exception("创建深度模板测试纹理失败");
        //创建渲染目标纹理视图
        const CD3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc{D3D11_RTV_DIMENSION_TEXTURE2D};
        result = device->CreateRenderTargetView(targetTexture, &renderTargetViewDesc, &defaultRenderTargetView.p);
        if (FAILED(result))throw std::exception("创建渲染目标纹理视图失败");
        //创建深度模板测试的视图
        const CD3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc{D3D11_DSV_DIMENSION_TEXTURE2D};
        result = device->CreateDepthStencilView(depthStencilTexture, &depthStencilViewDesc, &defaultDepthStencilView.p);
        if (FAILED(result))throw std::exception("创建深度模板测试视图失败");
    }

    int Texture::GetWidth() const
    {
        return width;
    }
    int Texture::GetHeight() const
    {
        return height;
    }
    Vector2 Texture::GetSize() const
    {
        return Vector2{width, height};
    }
    CComPtr<ID3D11ShaderResourceView> Texture::GetShaderResourceView()
    {
        return colorTextureSRV;
    }
    void Texture::SetPass(unsigned int startSlot) const
    {
        const auto context = GL::GetDeviceContext();
        context->PSSetShaderResources(startSlot, 1, &colorTextureSRV.p);
        context->PSSetSamplers(startSlot, 1, &samplerState.p);
    }

    void Texture::CreateDepthStencil(unsigned int width, unsigned int height, ID3D11Texture2D** renderTexture,
                                     ID3D11DepthStencilView** depthStencilView)
    {
        D3D11_TEXTURE2D_DESC texture2DDepthDescription = {};
        texture2DDepthDescription.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
        texture2DDepthDescription.Width = width;
        texture2DDepthDescription.Height = height;
        texture2DDepthDescription.ArraySize = 1;
        texture2DDepthDescription.MipLevels = 1;
        texture2DDepthDescription.SampleDesc = {1, 0};
        texture2DDepthDescription.BindFlags = D3D11_BIND_DEPTH_STENCIL;
        //创建深度纹理
        HRESULT result = GL::GetDevice()->CreateTexture2D(&texture2DDepthDescription, nullptr, renderTexture);
        if (FAILED(result))throw std::exception("创建深度模板纹理失败");
        //创建深度模板视图
        result = GL::GetDevice()->CreateDepthStencilView(*renderTexture, nullptr, depthStencilView);
        if (FAILED(result))throw std::exception("创建深度模板纹理视图失败");
        //创建深度纹理着色器资源视图
        //result = device->CreateShaderResourceView(depthTexture, nullptr, &texture2DDepthResourceView.p);
        //assert(result);
    }
    void Texture::CreateSamplerState(ID3D11SamplerState** samplerState)
    {
        D3D11_SAMPLER_DESC samplerDescription{};
        samplerDescription.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
        samplerDescription.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
        samplerDescription.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
        samplerDescription.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
        GL::GetDevice()->CreateSamplerState(&samplerDescription, samplerState);
    }

    void Texture::Transfer(Transferer& transferer)
    {
        Object::Transfer(transferer);

        TransferFieldInfo(width);
        TransferFieldInfo(height);
        TransferFieldInfoOf(format, int);
    }
}
