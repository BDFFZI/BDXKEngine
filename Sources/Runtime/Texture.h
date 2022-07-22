#pragma once
#include "GL.h"
#include "ObjectPtr.h"

namespace BDXKEngine {
	class TextureEditor;
	class Texture :public Object
	{
		friend TextureEditor;
	protected:
		Texture(std::wstring name = L"New Texture");

		void SetTextureView(ID3D11ShaderResourceView* textureView) {
			this->textureView.p = textureView;
		}
	private:
		CComPtr<ID3D11ShaderResourceView> textureView;
	};

	class TextureEditor {
	protected:
		CComPtr<ID3D11ShaderResourceView> GetResourceView(ObjectPtr<Texture> texture) {
			return texture->textureView;
		}
	};
}

