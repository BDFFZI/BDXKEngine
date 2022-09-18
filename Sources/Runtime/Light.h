#pragma once
#include "GameObject.h"
#include "Color.h"
#include "GLLayout.h"
#include "Resources.h"
#include "Texture2D.h"
#include "TextureCube.h"
#include "Graphics.h"
#include "Renderer.h"
#include "RendererManager.h"
#include "Transform.h"
#include "GraphicsSettings.h"

namespace BDXKEngine {
	class LightEditor;
	class Light :public Component, public RenderObjectHandler, RendererManager
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


		ObjectPtr<Texture2D> shadowMap;
		ObjectPtr<TextureCube> shadowMapCube;
	private:
		static std::vector<ObjectPtr<Light>> lights;

		LightType type = LightType::Directional;
		Color color = Color::white;
		float intensity = 1;
		RenderMode renderMode = RenderMode::Important;

		LightInfo GetLightInfo(int order = 0);
		ShadowInfo GetShadowInfo();
		ObjectPtr<Texture> GetShadowMap();

		void Awake()override;
		void OnRenderObject()override;
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

