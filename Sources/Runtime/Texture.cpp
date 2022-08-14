#include "Texture.h"

void BDXKEngine::Texture::CreateDepthStencil(unsigned int width, unsigned int height, ID3D11Texture2D** renderTexture, ID3D11DepthStencilView** depthStencilView)
{
	D3D11_TEXTURE2D_DESC texture2DDepthDescription = D3D11_TEXTURE2D_DESC{};
	texture2DDepthDescription.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	texture2DDepthDescription.Width = width;
	texture2DDepthDescription.Height = height;
	texture2DDepthDescription.ArraySize = 1;
	texture2DDepthDescription.MipLevels = 1;
	texture2DDepthDescription.SampleDesc = { 1,0 };
	texture2DDepthDescription.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	//�����������
	HRESULT result = device->CreateTexture2D(&texture2DDepthDescription, nullptr, renderTexture);
	assert(SUCCEEDED(result));
	//�������ģ����ͼ
	result = device->CreateDepthStencilView(*renderTexture, nullptr, depthStencilView);
	assert(SUCCEEDED(result));
	//�������������ɫ����Դ��ͼ
	//result = device->CreateShaderResourceView(depthTexture, nullptr, &texture2DDepthResourceView.p);
	//assert(SUCCEEDED(result));
}

void BDXKEngine::Texture::CreateSamplerState(ID3D11SamplerState** samplerState)
{
	D3D11_SAMPLER_DESC samplerDescription{};
	samplerDescription.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDescription.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDescription.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDescription.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	device->CreateSamplerState(&samplerDescription, samplerState);
}
