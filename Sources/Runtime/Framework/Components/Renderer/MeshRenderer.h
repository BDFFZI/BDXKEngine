#pragma once
#include "Framework/Components/Renderer/Renderer.h"
#include "Function/Graphics.h"

namespace BDXKEngine {
	class MeshRenderer :public Renderer
	{
	public:
		void SetMesh(ObjectPtr<Mesh> mesh) override
		{
			Renderer::SetMesh(mesh);
		}
	};
}
