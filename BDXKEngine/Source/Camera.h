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
		background = Color::gray;
		clippingPlanes = { 0.003f,1 };
		fieldOfView = 60;
		size = 1;
	}

	void OnRenderObject()override
	{
		Vector3 viewSize = Screen::GetSize() / 1080 * 20;

		Matrix4x4 worldToCamera = transform->GetWorldToLocalMatrix();
		Matrix4x4 cameraToView = {
			//控制画面缩放并避免受窗口大小影响
			3600 / fieldOfView / viewSize.x,0,0,0,
			0,3600 / fieldOfView / viewSize.y,0,0,
			//利用裁剪功能避免z过小导致 xy / 0 的情况
			0,0,1,-clippingPlanes.x,
			//利用齐次坐标中的w分量实现近大远小公式 xy / z
			0,0,1,0
		};

		//获取所有渲染物体
		std::vector<Renderer*> renderers = FindObjectsOfType<Renderer*>();
		for (Renderer* renderer : renderers)
		{
			Transform* rendererTransform = renderer->GetGameObject()->GetTransform();
			Matrix4x4 localToWorld = rendererTransform->GetLocalToWorldMatrix();

			//设置渲染环境
			Graphics::Clear(background);
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

