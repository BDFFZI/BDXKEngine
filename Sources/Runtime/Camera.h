#pragma once
#include "Graphics.h"
#include "GameObject.h"
#include "Component.h"
#include "Transform.h"
#include "Renderer.h"
#include "Screen.h"
#include "Light.h"

namespace BDXKEngine {
	enum class ClearFlags {
		Not,
		Color,
		Skybox,
	};

	enum class Projection {
		Orthographic,
		Perspective
	};

	class Camera :public Component, public RenderObjectEvent, RendererEditor, LightEditor
	{
	public:
		Camera();

		void SetClearFlags(ClearFlags clearFlags);
		void SetBackground(Color color);
	private:
		ClearFlags clearFlags = ClearFlags::Color;
		Projection projection = Projection::Orthographic;//TODO
		Color background = Color::gray;
		Vector2 clippingPlanes = { 0.003f,1 };//有bug，y的值没有用 TODO
		float fieldOfView = 60;
		float size = 1;//TODO

		ObjectPtr<Transform> transform{};

		void OnAwake() override
		{
			Component::OnAwake();

			transform = GetGameObject()->GetTransform();
		}

		void OnRenderObject()override;
	};
}
