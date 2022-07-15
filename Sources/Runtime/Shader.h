#pragma once
#include "GLLayout.h"

namespace BDXKEngine {
	enum class PassType {
		Always,
		ForwardBase,
		ForwardAdd,
	};

	class ShaderEditor;
	class Shader
	{
		friend ShaderEditor;
	public:
		Shader(char* vertexShaderCso, char* pixelShaderCso, PassType passType, bool useBlend = true)
		{
			GL::CreateVertexShader(vertexShaderCso,
				(D3D11_INPUT_ELEMENT_DESC*)VertexDescription, ARRAYSIZE(VertexDescription),
				&vertexShader.p, &inputLayout.p);
			GL::CreatePixelShader(pixelShaderCso,
				&pixelShader.p);

			this->passType = passType;

			if (useBlend) GL::CreateBlendState(D3D11_BLEND_SRC_ALPHA, D3D11_BLEND_INV_SRC_ALPHA, &blendState.p);
			else GL::CreateBlendState(D3D11_BLEND_ONE, D3D11_BLEND_ONE, &blendState.p);

			GL::CreateDepthStencilState(0, true, true, &depthStencilState.p);
		}

		PassType GetPassType();
	private:
		PassType passType;
		CComPtr<ID3D11InputLayout> inputLayout;
		CComPtr<ID3D11BlendState> blendState;
		CComPtr<ID3D11DepthStencilState> depthStencilState;

		CComPtr<ID3D11VertexShader> vertexShader;
		CComPtr<ID3D11PixelShader> pixelShader;
	};

	class ShaderEditor {
	protected:
		static void SetPass(Shader* shader)
		{
			GL::SetShader(shader->vertexShader, shader->inputLayout, shader->pixelShader);
			GL::SetBlendState(shader->blendState);
			GL::SetDepthStencilState(shader->depthStencilState);
		}

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
}
