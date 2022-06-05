#pragma once
#include<functional>
#include"Component.h"
#include"Transform.h"
#include"Color.h"
#include "GameObject.h"
#include "Graphics2D.h"

class Renderer :
	public Component,
	public StartEvent,
	public OnRenderObjectEvent
{
public:
	std::function<void()> render = NULL;
	Color color = Color::white;
private:
	Transform* transform = NULL;

	void Start()override;

	void OnRenderObject() override;
};
