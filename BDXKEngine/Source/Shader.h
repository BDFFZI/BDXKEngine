#pragma once
#include "GLLayout.h"

class ShaderEditor;
class Shader
{
	friend ShaderEditor;
public:
	Shader()
	{
		GL::CreateVertexShader("C:/Users/BDFFZI/Desktop/BDXKEngine/Shader/VertexShader.cso",
			(D3D11_INPUT_ELEMENT_DESC*)VertexDescription, ARRAYSIZE(VertexDescription),
			&vertexShader.p, &inputLayout.p);
		GL::CreatePixelShader("C:/Users/BDFFZI/Desktop/BDXKEngine/Shader/PixelShader.cso",
			&pixelShader.p);
	}
private:
	CComPtr<ID3D11InputLayout> inputLayout;
	CComPtr<ID3D11VertexShader> vertexShader;
	CComPtr<ID3D11PixelShader> pixelShader;
};

class ShaderEditor {
protected:
	static CComPtr<ID3D11InputLayout> GetInputLayout(Shader* shader)
	{
		return shader->inputLayout;
	}

	static CComPtr<ID3D11VertexShader> GetVertexShader(Shader* shader)
	{
		return shader->vertexShader;
	}

	static CComPtr<ID3D11PixelShader> GetPixelShader(Shader* shader)
	{
		return shader->pixelShader;
	}
};

