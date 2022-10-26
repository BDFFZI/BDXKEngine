#pragma once
#include <vector>
#include "Texture.h"

namespace BDXKEngine {
	/// <summary>
	/// 顺序为x,-x,y,-y,z,-z
	/// </summary>
	class TextureCube :public Texture
	{
		friend GL;
	public:
		static ObjectPtr<TextureCube> Create(int width, int height);


	private:
		std::vector<CComPtr<ID3D11RenderTargetView>> renderTextureRTV = {};
		
		void PreAwake()override;
	};
}

