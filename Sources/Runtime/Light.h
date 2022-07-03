#pragma once
#include "Component.h"
#include "Color.h"
#include "GLLayout.h"

class Light :public Component
{
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
private:
	LightType type = LightType::Directional;
	Color color = Color::white;
	float intensity = 1;
	RenderMode renderMode = RenderMode::Important;
};

