#include "Texture.h"

namespace BDXKEngine
{
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
    CComPtr<ID3D11ShaderResourceView> Texture::GetResourceView()
    {
        return colorTextureSRV;
    }
    CComPtr<ID3D11SamplerState> Texture::GetSamplerState()
    {
        return samplerState;
    }
    void Texture::Transfer(Transferrer& transferrer)
    {
        Object::Transfer(transferrer);

        transferrer.TransferInt(nameof(width), width);
        transferrer.TransferInt(nameof(height), height);
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
        HRESULT result = device->CreateTexture2D(&texture2DDepthDescription, nullptr, renderTexture);
        assert(SUCCEEDED(result));
        //创建深度模板视图
        result = device->CreateDepthStencilView(*renderTexture, nullptr, depthStencilView);
        assert(SUCCEEDED(result));
        //创建深度纹理着色器资源视图
        //result = device->CreateShaderResourceView(depthTexture, nullptr, &texture2DDepthResourceView.p);
        //assert(SUCCEEDED(result));
    }
    void Texture::CreateSamplerState(ID3D11SamplerState** samplerState)
    {
        D3D11_SAMPLER_DESC samplerDescription{};
        samplerDescription.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
        samplerDescription.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
        samplerDescription.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
        samplerDescription.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
        device->CreateSamplerState(&samplerDescription, samplerState);
    }

    CComPtr<ID3D11ShaderResourceView> TextureEditor::GetShaderResourceView(const ObjectPtr<Texture> texture)
    {
        return texture->colorTextureSRV;
    }
}
