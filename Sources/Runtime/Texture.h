#pragma once
#include"GL.h"

namespace BDXKEngine {
	class TextureEditor;
	class Texture
	{
		friend TextureEditor;
	protected:
		void SetTextureView(ID3D11ShaderResourceView* textureView) {
			this->textureView.p = textureView;
		}
	private:
		CComPtr<ID3D11ShaderResourceView> textureView;
	};

	class TextureEditor {
	protected:
		CComPtr<ID3D11ShaderResourceView> GetResourceView(Texture* texture) {
			return texture->textureView;
		}
	};
}

