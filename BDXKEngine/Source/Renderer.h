#pragma once
#include "Component.h"
#include "Graphics.h"

class RendererEditor;
class Renderer : public Component
{
	friend RendererEditor;
public:
	Shader* GetShader()
	{
		return shader;
	}
	void SetShader(Shader* shader)
	{
		this->shader = shader;
	}
protected:
	virtual void OnRender() = 0;
private:
	Shader* shader;
};

class RendererEditor {
protected:
	static void Render(Renderer* renderer)
	{
		renderer->OnRender();
	}
};
