#include "Texture2D.h"
#include "GL.h"

namespace BDXKEngine {
	Texture2D::Texture2D() : Texture(L"New Texture2D")
	{

	}
	Texture2D::Texture2D(unsigned int width, unsigned int height) : Texture2D()
	{
		HRESULT result;

		//创建渲染目标纹理
		texture2DDescription = D3D11_TEXTURE2D_DESC{};
		texture2DDescription.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		texture2DDescription.Width = width;
		texture2DDescription.Height = height;
		texture2DDescription.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
		texture2DDescription.ArraySize = 1;
		texture2DDescription.SampleDesc = { 1,0 };
		result = device->CreateTexture2D(&texture2DDescription, nullptr, &texture2D.p);
		assert(SUCCEEDED(result));
		//创建渲染目标视图
		D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc{};
		renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
		result = device->CreateRenderTargetView(texture2D, &renderTargetViewDesc, &texture2DView.p);
		assert(SUCCEEDED(result));
		//创建深度测试纹理
		D3D11_TEXTURE2D_DESC texture2DDepthDescription = D3D11_TEXTURE2D_DESC{};
		texture2DDepthDescription.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		texture2DDepthDescription.Width = width;
		texture2DDepthDescription.Height = height;
		texture2DDepthDescription.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		texture2DDepthDescription.ArraySize = 1;
		texture2DDepthDescription.SampleDesc = { 1,0 };
		result = device->CreateTexture2D(&texture2DDepthDescription, nullptr, &texture2DDepth.p);
		assert(SUCCEEDED(result));
		//创建深度测试视图
		D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc{};
		depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		result = device->CreateDepthStencilView(texture2DDepth, &depthStencilViewDesc, &texture2DDepthView.p);
		assert(SUCCEEDED(result));
		//创建着色器资源视图
		CComPtr<ID3D11ShaderResourceView> resourceView = nullptr;
		D3D11_SHADER_RESOURCE_VIEW_DESC resourceDescription{};
		resourceDescription.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		resourceDescription.Format = texture2DDescription.Format;
		resourceDescription.Texture2D = D3D11_TEX2D_SRV{};
		result = device->CreateShaderResourceView(texture2D, nullptr, &resourceView.p);
		assert(SUCCEEDED(result));
		SetTextureView(resourceView);
	}
	Texture2D::Texture2D(Color color) :Texture2D(1, 1)
	{
		ObjectPtr<Texture2D> renderTarget = this;
		renderTarget.SuppressDestroy();
		GL::SetRenderTarget(renderTarget);
		GL::Clear(true, true, color);
		GL::SetRenderTarget(nullptr);
	}
	Texture2D::Texture2D(char* filePath) :Texture2D() {

	}

	unsigned int Texture2D::GetWidth()
	{
		return texture2DDescription.Width;
	}
	unsigned int Texture2D::GetHeight()
	{
		return texture2DDescription.Height;
	}
}
