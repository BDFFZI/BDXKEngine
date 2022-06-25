#pragma once
#include "Renderer.h"
#include "Graphics.h"

class MeshRenderer :public Renderer
{
public:
	Mesh* GetMesh()
	{
		return mesh;
	}
	void SetMesh(Mesh* mesh)
	{
		this->mesh = mesh;
	}
protected:
	void OnRender()override
	{
		if (mesh == NULL || GetShader() == NULL)
			return;
		Graphics::RenderMesh(mesh, GetShader());
	}
private:
	Mesh* mesh;
};

