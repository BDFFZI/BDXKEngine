#pragma once
#include "GL.h"
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

	class Shader :public Object, GL
	{
		friend GL;
	public:
		static ObjectPtr<Shader> Create(std::wstring vertexShaderhlsl, std::wstring pixelShaderhlsl, PassType passType);

		PassType GetPassType();
		Blend GetBlend();
		ZTest GetZTest();


		void SetPassType(PassType passType);
		void SetBlend(Blend blend);
		void SetZTest(ZTest zTest);
	private:
		std::wstring vertexShaderhlsl;
		std::wstring pixelShaderhlsl;
		PassType passType;
		Blend blend;
		ZTest zTest;

		CComPtr<ID3D11BlendState> blendState;
		CComPtr<ID3D11DepthStencilState> depthStencilState;

		CComPtr<ID3D11InputLayout> inputLayout;
		CComPtr<ID3D11VertexShader> vertexShader;
		CComPtr<ID3D11PixelShader> pixelShader;


		void Transfer(TransferBase& transfer)override;
		void Awake()override;
	};
}
