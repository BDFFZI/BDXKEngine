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
		if (mesh == NULL)
			return;
		Graphics::DrawMeshNow(mesh);
	}
private:
	Mesh* mesh = nullptr;
};

