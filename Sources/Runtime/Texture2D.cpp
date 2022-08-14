#include "Texture2D.h"
#include "GL.h"

namespace BDXKEngine {
	ObjectPtr<Texture2D> Texture2D::Create(unsigned int width, unsigned int height)
	{
		Texture2D* texture2D = Instantiate<Texture2D>(nullptr);

		HRESULT result;

		//创建渲染纹理
		D3D11_TEXTURE2D_DESC renderTextureDesc = D3D11_TEXTURE2D_DESC{};
		renderTextureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		renderTextureDesc.Width = width;
		renderTextureDesc.Height = height;
		renderTextureDesc.ArraySize = 1;
		renderTextureDesc.MipLevels = 1;
		renderTextureDesc.SampleDesc = { 1,0 };
		renderTextureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
		texture2D->renderTextureDesc = renderTextureDesc;

		result = device->CreateTexture2D(&renderTextureDesc, nullptr, &texture2D->renderTexture.p);
		assert(SUCCEEDED(result));
		//创建渲染目标视图，使其可作为画布承载渲染结果
		D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc{};
		renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
		result = device->CreateRenderTargetView(texture2D->renderTexture, &renderTargetViewDesc, &texture2D->renderTextureRTV.p);
		assert(SUCCEEDED(result));
		//创建渲染纹理着色器资源视图，使其可作为贴图使用
		result = device->CreateShaderResourceView(texture2D->renderTexture, nullptr, &texture2D->renderTextureSRV.p);
		assert(SUCCEEDED(result));

		//创建深度纹理
		CreateDepthStencil(width, height, &texture2D->depthTexture.p, &texture2D->depthTextureDSV.p);

		//创建采样器
		CreateSamplerState(&texture2D->samplerState.p);

		return texture2D;
	}

	ObjectPtr<Texture2D> Texture2D::Create(Color color)
	{
		ObjectPtr<Texture2D> renderTarget = Create(1, 1);

		GL::SetRenderTarget(renderTarget);
		GL::Clear(true, true, color);
		GL::SetRenderTarget(nullptr);

		return renderTarget;
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
}
