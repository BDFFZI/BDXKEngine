#pragma once
#include"Texture.h"

class Texture2DEditor;
class Texture2D :public Texture
{
	friend Texture2DEditor;
public:
	Texture2D(unsigned int width, unsigned int height);

	Texture2D(char* filePath) {

	}

private:
	CComPtr<ID3D11Texture2D> texture2D;
};

class Texture2DEditor {
protected:
	static CComPtr<ID3D11Texture2D> GetGLTexture2D(Texture2D* texture)
	{
		return texture->texture2D;
	}
};
