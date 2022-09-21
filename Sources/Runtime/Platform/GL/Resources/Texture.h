#pragma once
#include "Platform/GL/GL.h"

namespace BDXKEngine {
	class Texture :public Object, protected GL
	{
		friend GL;
	protected:
		static void CreateDepthStencil(unsigned int width, unsigned int height, ID3D11Texture2D** renderTexture, ID3D11DepthStencilView** depthStencilView);
		static void CreateSamplerState(ID3D11SamplerState** samplerState);

		virtual CComPtr<ID3D11ShaderResourceView> GetResourceView() = 0;
		virtual CComPtr<ID3D11SamplerState> GetSamplerState() = 0;
	};
}

