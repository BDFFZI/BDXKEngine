#pragma once
#include "Function/Graphics.h"
#include "Framework/GameObject.h"
#include "Framework/Component.h"
#include "Framework/Components/Transform.h"
#include "Framework/Components/Renderer/Renderer.h"
#include "Function/Screen.h"
#include "Framework/Components/Renderer/Light.h"

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

	class RendererManager;
	class Camera :public Component, RendererManager, LightEditor
	{
		friend RendererManager;
	public:
		float GetAspectRatio();

		void SetClearFlags(ClearFlags clearFlags);
		void SetBackground(Color color);
		void SetNearClipPlane(float distance);
		void SetFarClipPlane(float distance);
	private:
		ClearFlags clearFlags = ClearFlags::Color;
		Projection projection = Projection::Perspective;
		Color background = Color::gray;
		float nearClipPlane = 0.3f;
		float farClipPlane = 1000;
		float fieldOfView = 60;
		float size = 5;

		ObjectPtr<Transform> transform{};

		void Awake() override;
		void Render();
	};
}
