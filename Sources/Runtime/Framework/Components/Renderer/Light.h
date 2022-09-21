#pragma once
#include "Framework/GameObject.h"
#include "Base/Color.h"
#include "Platform/GL/Resources/GLLayout.h"
#include "Function/Resources.h"
#include "Platform/GL/Resources/Texture2D.h"
#include "Platform/GL/Resources/TextureCube.h"
#include "Function/Graphics.h"
#include "Framework/Components/Renderer/Renderer.h"
#include "Framework/Components/Renderer/RendererManager.h"
#include "Framework/Components/Transform.h"
#include "Framework/Settings/GraphicsSettings.h"

namespace BDXKEngine {
	class LightEditor;
	class Light :public Component, public PreRenderHandler, RendererManager
	{
		friend LightEditor;

	public:
		LightType GetType();
		Color GetColor();
		float GetIntensity();
		RenderMode GetRenderMode();

		void SetLightType(LightType type);
		void SetColor(Color color);
		void SetIntensity(float intensity);
	private:
		static std::vector<ObjectPtr<Light>> lights;

		ObjectPtr<Texture2D> shadowMap;
		ObjectPtr<TextureCube> shadowMapCube;
		LightType type = LightType::Directional;
		Color color = Color::white;
		float intensity = 1;
		RenderMode renderMode = RenderMode::Important;

		LightInfo GetLightInfo(int order = 0);
		ShadowInfo GetShadowInfo();
		ObjectPtr<Texture> GetShadowMap();

		void Awake()override;
		void OnPreRender()override;
		void Destroy()override;
	};

	class LightEditor {
	protected:
		static LightInfo GetLightInfo(ObjectPtr<Light> light, int order = 0);
		static ShadowInfo GetShadowInfo(ObjectPtr<Light> light);
		static ObjectPtr<Texture> GetShadowMap(ObjectPtr<Light> light);
		static std::vector<ObjectPtr<Light>>& GetLights();
	};
}

