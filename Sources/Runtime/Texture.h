#pragma once
#include "GL.h"

namespace BDXKEngine {
	class Texture :public Object, protected GL
	{
		friend GL;
	protected:
		static void CreateDepthStencil(unsigned int width, unsigned int height, ID3D11Texture2D** renderTexture, ID3D11DepthStencilView** depthStencilView);
		
		Texture(std::wstring name = L"New Texture");

		virtual CComPtr<ID3D11ShaderResourceView> GetResourceView() = 0;
	};
}

