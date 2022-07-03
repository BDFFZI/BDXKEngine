#pragma once
#include "Graphics.h"
#include "GameObject.h"
#include "Component.h"
#include "TickEvent.h"
#include "Transform.h"
#include "Renderer.h"
#include "Screen.h"
#include "Light.h"

enum class ClearFlags {
	Not,
	Color,
	Skybox,
};

enum class Projection {
	Orthographic,
	Perspective
};

class Camera :public Component, public AwakeEvent, public RenderObjectEvent, RendererEditor
{
public:
	void SetClearFlags(ClearFlags clearFlags);
	void SetBackground(Color color);
private:
	ClearFlags clearFlags = ClearFlags::Color;
	Projection projection = Projection::Orthographic;//TODO
	Color background = Color::gray;
	Vector2 clippingPlanes = { 0.003f,1 };//有bug，y的值没有用 TODO
	float fieldOfView = 60;
	float size = 1;//TODO

	Transform* transform{};

	void OnAwake() override
	{
		transform = GetGameObject()->GetTransform();
	}

	void OnRenderObject()override;
};

