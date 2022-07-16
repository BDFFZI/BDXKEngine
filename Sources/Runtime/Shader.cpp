#include "Shader.h"

namespace BDXKEngine {
	Shader::Shader(const wchar_t* vertexShaderhlsl, const wchar_t* pixelShaderhlsl, PassType passType)
	{
		GL::CreateVertexShader(
			vertexShaderhlsl,
			(D3D11_INPUT_ELEMENT_DESC*)VertexDescription,
			ARRAYSIZE(VertexDescription),
			&vertexShader.p, &inputLayout.p
		);
		GL::CreatePixelShader(
			pixelShaderhlsl,
			&pixelShader.p
		);

		SetPassType(passType);
		SetBlend({});
		SetZTest({});
	}

	PassType Shader::GetPassType()
	{
		return passType;
	}
	Blend Shader::GetBlend()
	{
		return blend;
	}
	ZTest Shader::GetZTest()
	{
		return zTest;
	}

	void Shader::SetPassType(PassType passType)
	{
		this->passType = passType;
	}
	void Shader::SetBlend(Blend blend)
	{
		this->blend = blend;
		GL::UpdateBlend(&this->blend);
	}
	void Shader::SetZTest(ZTest zTest)
	{
		this->zTest = zTest;
		GL::UpdateZTest(&this->zTest);
	}
}