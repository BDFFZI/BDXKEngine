#include "Texture2D.h"
#include "Graphics.h"

namespace BDXKEngine {
	Texture2D::Texture2D(unsigned int width, unsigned int height)
	{
		ID3D11ShaderResourceView* resourceView;
		GL::CreateTexture2D(width, height, &texture2D.p, &resourceView);
		SetTextureView(resourceView);
	}

	Texture2D::Texture2D(Color color) :Texture2D(1, 1)
	{
		Graphics::SetRenderTarget(this);
		GL::Begin(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		GL::Clear(true, true, Color::white);
		GL::End();
		Graphics::SetRenderTarget(nullptr);
	}
}
