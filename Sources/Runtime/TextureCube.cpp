#include "TextureCube.h"
using namespace BDXKEngine;

ObjectPtr<TextureCube> BDXKEngine::TextureCube::Create(int width, int height)
{
	ObjectPtr<TextureCube> textureCube = Instantiate<TextureCube>(nullptr);

	HRESULT result = 0;
	//创建渲染纹理
	D3D11_TEXTURE2D_DESC renderTextureDesc = D3D11_TEXTURE2D_DESC{};
	renderTextureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	renderTextureDesc.Width = width;
	renderTextureDesc.Height = height;
	renderTextureDesc.ArraySize = 6;
	renderTextureDesc.MipLevels = 1;
	renderTextureDesc.SampleDesc = { 1,0 };
	renderTextureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
	renderTextureDesc.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE;
	textureCube->renderTextureDesc = renderTextureDesc;

	result = device->CreateTexture2D(&renderTextureDesc, nullptr, &textureCube->renderTexture.p);
	assert(SUCCEEDED(result));
	//创建渲染纹理资源视图
	D3D11_SHADER_RESOURCE_VIEW_DESC SMViewDesc = {};
	SMViewDesc.Format = renderTextureDesc.Format;
	SMViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
	SMViewDesc.TextureCube = { 0,1 };
	result = device->CreateShaderResourceView(textureCube->renderTexture, &SMViewDesc, &textureCube->renderTextureSRV.p);
	assert(SUCCEEDED(result));
	//创建渲染纹理渲染目标视图
	for (unsigned int index = 0; index < 6; index++)
	{
		D3D11_RENDER_TARGET_VIEW_DESC renderTargetDesc = {};
		renderTargetDesc.Format = renderTextureDesc.Format;
		renderTargetDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DARRAY;
		renderTargetDesc.Texture2DArray = { 0,index,1 };

		CComPtr<ID3D11RenderTargetView> renderTargetView;
		result = device->CreateRenderTargetView(textureCube->renderTexture, &renderTargetDesc, &renderTargetView.p);
		assert(SUCCEEDED(result));
		textureCube->renderTextureRTV.push_back(renderTargetView);
	}

	//创建深度纹理
	CreateDepthStencil(width, height, &textureCube->depthTexture.p, &textureCube->depthTextureDSV.p);

	//创建采样器
	CreateSamplerState(&textureCube->samplerState.p);

	return textureCube;
}

unsigned int TextureCube::GetWidth()
{
	return renderTextureDesc.Width;
}

unsigned int TextureCube::GetHeight()
{
	return renderTextureDesc.Height;
}

CComPtr<ID3D11ShaderResourceView> TextureCube::GetResourceView()
{
	return renderTextureSRV;
}

CComPtr<ID3D11SamplerState> BDXKEngine::TextureCube::GetSamplerState()
{
	return samplerState;
}
