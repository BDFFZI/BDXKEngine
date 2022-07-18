#pragma once
#include "GameObject.h"
#include "Color.h"
#include "GLLayout.h"
#include "RenderEvent.h"
#include "Texture2D.h"
#include "Graphics.h"
#include "Renderer.h"
#include "Transform.h"

namespace BDXKEngine {
	class LightEditor;
	class Light :public Component, public RenderObjectEvent, RendererEditor
	{
		friend LightEditor;

	public:
		LightType GetType()
		{
			return type;
		}
		Color GetColor()
		{
			return color;
		}
		float GetIntensity()
		{
			return intensity;
		}
		RenderMode GetRenderMode()
		{
			return renderMode;
		}

		void SetLightType(LightType type) {
			this->type = type;
		}
		void SetColor(Color color)
		{
			this->color = color;
		}
		void SetIntensity(float intensity)
		{
			this->intensity = intensity;
		}

		Light()
		{
			lights.push_back(this);
			shadow = new Texture2D{ 1024,1024 };
		}
	private:
		static std::vector<ObjectPtr<Light>> lights;

		LightType type = LightType::Directional;
		Color color = Color::white;
		float intensity = 1;
		RenderMode renderMode = RenderMode::Important;
		Texture2D* shadow;

		void OnRenderObject()override
		{
			Graphics::SetRenderTarget(shadow);

			//“ı”∞
			//std::vector<Renderer*>& renderers = RendererEditor::GetRenderers();


			Graphics::SetRenderTarget(nullptr);
		}
	};

	class LightEditor {
	protected:
		static LightInfo GetLightInfo(ObjectPtr<Light> light, int order = 0)
		{
			ObjectPtr<Transform> lightTransform = light->GetGameObject()->GetTransform();
			LightInfo lightInfo{
				Vector4(lightTransform->GetPosition(),1),
				Vector4(lightTransform->GetFront().GetNormalized(),1),
				light->GetColor() * light->GetIntensity(),
				light->GetType(),
				light->GetRenderMode(),
				order
			};

			return lightInfo;
		}
		static std::vector<ObjectPtr<Light>>& GetLights()
		{
			return Light::lights;
		}
	};
}

