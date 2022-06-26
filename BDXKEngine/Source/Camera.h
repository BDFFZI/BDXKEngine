#pragma once
#include "Graphics.h"
#include "GameObject.h"
#include "Component.h"
#include "TickEvent.h"
#include "Transform.h"
#include "Renderer.h"
#include "Screen.h"

enum class ClearFlags {
	Not,
	Color,
};

enum class Projection {
	Orthographic,
	Perspective
};

class Camera :public Component, public AwakeEvent, public RenderObjectEvent, RendererEditor
{
public:


private:
	ClearFlags clearFlags{};
	Projection projection{};
	Color background{};
	Vector2 clippingPlanes{};
	float size{};
	float fieldOfView{};

	Transform* transform{};

	void OnAwake() override
	{
		transform = GetGameObject()->GetTransform();
		clearFlags = ClearFlags::Color;
		projection = Projection::Orthographic;
		background = Color::white;
		clippingPlanes = { 0.003f,1000 };
		fieldOfView = 60;
		size = 1;
	}

	void OnRenderObject()override
	{
		Vector3 viewSize = Screen::GetSize() / 1080 * 10;

		Matrix4x4 worldToCamera = transform->GetWorldToLocalMatrix();
		Matrix4x4 cameraToView = Matrix4x4::Translate({ 0,0,-clippingPlanes.x })
			* Matrix4x4::Scale({ 1 / viewSize.x,1 / viewSize.y,1 / clippingPlanes.y });

		//获取所有渲染物体
		std::vector<Renderer*> renderers = FindObjectsOfType<Renderer*>();
		for (Renderer* renderer : renderers)
		{
			Transform* rendererTransform = renderer->GetGameObject()->GetTransform();
			Matrix4x4 localToWorld = rendererTransform->GetLocalToWorldMatrix();

			//设置渲染环境
			Graphics::Clear();
			Graphics::UpdateMatrix({
		localToWorld,
		worldToCamera,
		cameraToView
				});
			//渲染
			RendererEditor::Render(renderer);
		}
	}
};

