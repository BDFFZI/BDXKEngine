#pragma once
#include "GL.h"

class Mesh
{
public:
	Mesh();

	void Render();
private:
	CComPtr<ID3D11Buffer> vertexBuffer;
	CComPtr<ID3D11Buffer> indexBuffer;

	CComPtr<ID3D11VertexShader> vertexShader;
	CComPtr<ID3D11InputLayout> inputLayout;

	CComPtr<ID3D11PixelShader> pixelShader;
};

