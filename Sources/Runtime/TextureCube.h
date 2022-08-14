#pragma once
#include <vector>
#include "Texture.h"

namespace BDXKEngine {
	/// <summary>
	/// Ë³ÐòÎªx,-x,y,-y,z,-z
	/// </summary>
	class TextureCube :public Texture
	{
		friend GL;
	public:
		static ObjectPtr<TextureCube> Create(int width, int height);

		unsigned int GetWidth();
		unsigned int GetHeight();
	private:
		CComPtr<ID3D11SamplerState> samplerState;

		D3D11_TEXTURE2D_DESC renderTextureDesc;
		CComPtr<ID3D11Texture2D> renderTexture = nullptr;
		CComPtr<ID3D11ShaderResourceView> renderTextureSRV = nullptr;
		std::vector<CComPtr<ID3D11RenderTargetView>> renderTextureRTV = {};

		CComPtr<ID3D11Texture2D> depthTexture = nullptr;
		CComPtr<ID3D11DepthStencilView> depthTextureDSV = {};

		CComPtr<ID3D11ShaderResourceView> GetResourceView()override;
		CComPtr<ID3D11SamplerState> GetSamplerState()override;
	};
}

