#pragma once
#include "ObjectPtr.h"
#include "GLLayout.h"
#include "Blend.h"
#include "ZTest.h"

namespace BDXKEngine {
	enum class PassType {
		ForwardBase,
		ForwardAdd,
		ShadowCaster
	};

	class ShaderEditor;
	class Shader :public Object
	{
		friend ShaderEditor;
	public:
		Shader(std::wstring vertexShaderhlsl, std::wstring pixelShaderhlsl, PassType passType);

		PassType GetPassType();
		Blend GetBlend();
		ZTest GetZTest();


		void SetPassType(PassType passType);
		void SetBlend(Blend blend);
		void SetZTest(ZTest zTest);
	private:
		PassType passType;
		Blend blend;
		ZTest zTest;

		CComPtr<ID3D11InputLayout> inputLayout;
		CComPtr<ID3D11VertexShader> vertexShader;
		CComPtr<ID3D11PixelShader> pixelShader;
	};

	class ShaderEditor {
	protected:
		static void SetPass(ObjectPtr<Shader> shader)
		{
			GL::SetShader(shader->vertexShader, shader->inputLayout, shader->pixelShader);
			GL::SetBlend(&shader->blend);
			GL::SetZTest(&shader->zTest);
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
