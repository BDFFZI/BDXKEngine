#pragma once
#include "GameObject.h"
#include "Color.h"
#include "GLLayout.h"
#include "RenderEvent.h"
#include "Texture2D.h"
#include "Graphics.h"
#include "Renderer.h"
#include "Transform.h"
#include "GraphicsSettings.h"

namespace BDXKEngine {
	class LightEditor;
	class Light :public Component, public RenderObjectEvent, RendererEditor, ShaderEditor
	{
		friend LightEditor;

	public:
		Light();

		LightType GetType();
		Color GetColor();
		float GetIntensity();
		RenderMode GetRenderMode();

		void SetLightType(LightType type);
		void SetColor(Color color);
		void SetIntensity(float intensity);

		ObjectPtr<Texture2D> shadowMap;
	private:
		static std::vector<ObjectPtr<Light>> lights;

		LightType type = LightType::Directional;
		Color color = Color::white;
		float intensity = 1;
		RenderMode renderMode = RenderMode::Important;

		LightInfo GetLightInfo(int order = 0);
		ShadowInfo GetShadowInfo();

		void OnAwake()override;
		void OnRenderObject()override;
		void OnDestroy()override;
	};

	class LightEditor {
	protected:
		static ObjectPtr<Texture2D> GetShadowMap(ObjectPtr<Light> light);
		static LightInfo GetLightInfo(ObjectPtr<Light> light, int order = 0);
		static ShadowInfo GetShadowInfo(ObjectPtr<Light> light);
		static std::vector<ObjectPtr<Light>>& GetLights();
	};
}

