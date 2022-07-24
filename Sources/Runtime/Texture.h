#pragma once
#include <d3d11_1.h>
#include <dxgi.h>
#include <atlbase.h>
#include "ObjectPtr.h"
#include "GL.h"

namespace BDXKEngine {
	class Texture :public Object, protected GL
	{
		friend GL;
	protected:
		Texture(std::wstring name = L"New Texture");

		void SetTextureView(CComPtr<ID3D11ShaderResourceView> textureView) {
			this->textureView = textureView;
		}
	private:
		CComPtr<ID3D11ShaderResourceView> textureView = nullptr;
	};
}

