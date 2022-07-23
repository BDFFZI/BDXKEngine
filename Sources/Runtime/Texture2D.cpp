#include "Texture2D.h"
#include "Graphics.h"

namespace BDXKEngine {
	Texture2D::Texture2D() :Texture(L"New Texture2D")
	{
	}
	Texture2D::Texture2D(unsigned int width, unsigned int height) : Texture2D()
	{
		ID3D11ShaderResourceView* resourceView;
		GL::CreateTexture2D(width, height, &texture2D.p, &resourceView);
		SetTextureView(resourceView);
	}

	Texture2D::Texture2D(Color color) :Texture2D(1, 1)
	{
		GL::SetRenderTexture(texture2D);
		GL::Begin();
		GL::Clear(true, true, color);
		GL::End();
		GL::SetRenderTexture(nullptr);
	}
	Texture2D::Texture2D(char* filePath) :Texture2D() {

	}
}
