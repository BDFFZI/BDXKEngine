#include "Resources.h"

namespace BDXKEngine {
	ObjectPtr<Material> Resources::shadowMapMaterial = nullptr;
	ObjectPtr<Material> Resources::skyboxMaterial = nullptr;
	ObjectPtr<Material> Resources::blitMaterial = nullptr;
	ObjectPtr<Texture2D> Resources::whiteTexture = nullptr;

	ObjectPtr<Material> Resources::GetShadowMapMaterial()
	{
		return shadowMapMaterial;
	}
	ObjectPtr<Material> Resources::GetSkyboxMaterial()
	{
		return skyboxMaterial;
	}
	ObjectPtr<Material> Resources::GetBlitMaterial()
	{
		return blitMaterial;
	}
	ObjectPtr<Texture2D> Resources::GetWhiteTexture()
	{
		return whiteTexture;
	}

	Resources* BDXKEngine::Resources::Initialize(Window* window, GL* gl)
	{
		//������Ӱ��ͼ
		shadowMapMaterial = new Material{ {new Shader(
			GetResourcesPathW(Shaders, ShadowMap\\VertexShader.hlsl),
			GetResourcesPathW(Shaders, ShadowMap\\PixelShader.hlsl),
			PassType::ForwardBase
		)} };
		//������պ�
		skyboxMaterial = new Material{ {new Shader(
			GetResourcesPathW(Shaders, Skybox\\VertexShader.hlsl),
			GetResourcesPathW(Shaders, Skybox\\PixelShader.hlsl),
			PassType::ForwardBase
		)} };
		for (ObjectPtr<Shader> shader : skyboxMaterial->GetShaders())
		{
			ZTest ztest = {};
			ztest.write = false;
			ztest.operation = ZTest::Operation::Always;
			shader->SetZTest(ztest);
		}
		//λ�鴫��
		blitMaterial = new Material{ {
			new Shader(
				GetResourcesPathW(Shaders, Blit\\VertexShader.hlsl),
				GetResourcesPathW(Shaders, Blit\\PixelShader.hlsl),
				PassType::ForwardBase
			)
		} };
		//��ɫ��ͼ
		whiteTexture = new Texture2D(Color::white);

		window->AddMessageListener(OnWindowMessage);
		return new Resources();
	}

	std::map<size_t, std::function<ObjectPtr<Object>(std::wstring path)>> Resources::creator = {
		{
			typeid(Shader).hash_code(),
			[](std::wstring path) {
				return new Shader(
					path + L"/VertexShader.hlsl",
					path + L"/PixelShader.hlsl",
					PassType::ForwardBase
				);
			}
		},
		{

		}
	};

	void Resources::OnWindowMessage(Window* window, UINT messageSign, WPARAM wparameter, LPARAM lparameter)
	{
		switch (messageSign)
		{
		case WM_DESTROY:
			shadowMapMaterial = nullptr;
			skyboxMaterial = nullptr;
			blitMaterial = nullptr;
			whiteTexture = nullptr;
			break;
		}
	}
}