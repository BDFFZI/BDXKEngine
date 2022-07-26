#pragma once
#include"Texture.h"
#include"Color.h"

namespace BDXKEngine {
	class GL2D;
	class Texture2D :public Texture
	{
		friend GL;
		friend GL2D;
	public:
		Texture2D(unsigned int width, unsigned int height);
		Texture2D(Color color);
		Texture2D(char* filePath);

		unsigned int GetWidth();
		unsigned int GetHeight();
	private:
		Texture2D();

		D3D11_TEXTURE2D_DESC texture2DDescription;
		CComPtr<ID3D11Texture2D> texture2D = nullptr;
		CComPtr<ID3D11RenderTargetView> texture2DView = nullptr;
		CComPtr<ID3D11Texture2D> texture2DDepth = nullptr;
		CComPtr<ID3D11DepthStencilView> texture2DDepthView = nullptr;
	};
}
