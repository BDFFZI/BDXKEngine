#include "Texture2D.h"
#include "GL.h"

namespace BDXKEngine {
	Texture2D::Texture2D() : Texture(L"New Texture2D")
	{

	}
	Texture2D::Texture2D(CComPtr<ID3D11Texture2D> texture2D) : Texture2D()
	{
		this->texture2D = texture2D;
		SetTextureView(nullptr);
	}
	Texture2D::Texture2D(unsigned int width, unsigned int height) : Texture2D()
	{
		CComPtr<ID3D11ShaderResourceView> resourceView = nullptr;

		HRESULT result;

		texture2DDescription = CD3D11_TEXTURE2D_DESC(
			DXGI_FORMAT_R32G32B32A32_FLOAT,
			width,
			height);
		texture2DDescription.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
		result = device->CreateTexture2D(&texture2DDescription, nullptr, &texture2D.p);
		assert(SUCCEEDED(result));

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
		GL::Begin();
		GL::Clear(true, true, color);
		GL::End();
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

	CComPtr<IDXGISurface> Texture2D::GetDXGISurface()
	{
		CComPtr<IDXGISurface> dxgiSurface = nullptr;
		HRESULT result = texture2D->QueryInterface(&dxgiSurface.p);
		assert(SUCCEEDED(result));
		return dxgiSurface;
	}
}
