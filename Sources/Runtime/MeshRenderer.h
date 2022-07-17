#pragma once
#include "Renderer.h"
#include "Graphics.h"

namespace BDXKEngine {
	class MeshRenderer :public Renderer
	{
	public:
		void SetMesh(Mesh* mesh)
		{
			Renderer::SetMesh(mesh);
		}
	};
}
