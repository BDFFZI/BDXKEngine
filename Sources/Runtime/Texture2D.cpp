#include "Texture2D.h"

Texture2D::Texture2D(unsigned int width, unsigned int height)
{
	ID3D11ShaderResourceView* resourceView;
	GL::CreateTexture2D(width, height, &texture2D.p, &resourceView);
	SetTextureView(resourceView);
}
