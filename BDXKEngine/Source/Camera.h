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
			//���ƻ������Ų������ܴ��ڴ�СӰ��
			3600 / fieldOfView / viewSize.x,0,0,0,
			0,3600 / fieldOfView / viewSize.y,0,0,
			//���òü����ܱ���z��С���� xy / 0 �����
			0,0,1,-clippingPlanes.x,
			//������������е�w����ʵ�ֽ���ԶС��ʽ xy / z
			0,0,1,0
		};

		//��ȡ������Ⱦ����
		std::vector<Renderer*> renderers = FindObjectsOfType<Renderer*>();
		for (Renderer* renderer : renderers)
		{
			Transform* rendererTransform = renderer->GetGameObject()->GetTransform();
			Matrix4x4 localToWorld = rendererTransform->GetLocalToWorldMatrix();

			//������Ⱦ����
			Graphics::Clear(background);
			Graphics::UpdateMatrix({
		localToWorld,
		worldToCamera,
		cameraToView
				});
			//��Ⱦ
			RendererEditor::Render(renderer);
		}
	}
};

