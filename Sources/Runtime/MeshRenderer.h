#pragma once
#include "Renderer.h"
#include "Graphics.h"

namespace BDXKEngine {
	class MeshRenderer :public Renderer
	{
	public:
		MeshRenderer();

		void SetMesh(ObjectPtr<Mesh> mesh)
		{
			Renderer::SetMesh(mesh);
		}
	};
}
