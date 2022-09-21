#include "Texture2D.h"
#include "Platform/GL/GL.h"

namespace BDXKEngine {
	ObjectPtr<Texture2D> Texture2D::Create(unsigned int width, unsigned int height)
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
		texture2d.pixels = std::unique_ptr<Color[]>(new Color[]{ color });

		return Object::Instantiate<Texture2D>(&texture2d);
	}

	unsigned int Texture2D::GetWidth()
	{
		return renderTextureDesc.Width;
	}
	unsigned int Texture2D::GetHeight()
	{
		return renderTextureDesc.Height;
	}

	CComPtr<ID3D11ShaderResourceView> Texture2D::GetResourceView()
	{
		return renderTextureSRV;
	}
	CComPtr<ID3D11SamplerState> Texture2D::GetSamplerState()
	{
		return samplerState;
	}
	void Texture2D::Export(Exporter& exporter)
	{
		exporter.TransferInt(width);
		exporter.TransferInt(height);
		exporter.TransferBytes(pixels.get(), sizeof(Color) * width * height);
	}
	void Texture2D::Import(Importer& importer)
	{
		width = importer.TransferInt();
		height = importer.TransferInt();
		pixels = std::unique_ptr<Color[]>(new Color[width * height]);
		importer.TransferBytes(pixels.get(), sizeof(Color) * width * height);
	}
	void Texture2D::Awake()
	{
		HRESULT result;

		//创建渲染纹理
		renderTextureDesc = D3D11_TEXTURE2D_DESC{};
		renderTextureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		renderTextureDesc.Width = width;
		renderTextureDesc.Height = height;
		renderTextureDesc.ArraySize = 1;
		renderTextureDesc.MipLevels = 1;
		renderTextureDesc.SampleDesc = { 1,0 };
		renderTextureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
		D3D11_SUBRESOURCE_DATA resourceData = {};
		resourceData.pSysMem = pixels.get();
		resourceData.SysMemPitch = width * sizeof(Color);
		result = device->CreateTexture2D(&renderTextureDesc, &resourceData, &renderTexture.p);
		assert(SUCCEEDED(result));
		//创建渲染目标视图，使其可作为画布承载渲染结果
		D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc{};
		renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
		result = device->CreateRenderTargetView(renderTexture, &renderTargetViewDesc, &renderTextureRTV.p);
		assert(SUCCEEDED(result));
		//创建渲染纹理着色器资源视图，使其可作为贴图使用
		result = device->CreateShaderResourceView(renderTexture, nullptr, &renderTextureSRV.p);
		assert(SUCCEEDED(result));

		//创建深度纹理
		CreateDepthStencil(width, height, &depthTexture.p, &depthTextureDSV.p);

		//创建采样器
		CreateSamplerState(&samplerState.p);
	}
}
