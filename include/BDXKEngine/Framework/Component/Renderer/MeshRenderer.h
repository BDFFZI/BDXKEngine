#pragma once
#include "BDXKEngine/Framework/Component/Renderer/Renderer.h"
#include "BDXKEngine/Function/Graphics.h"

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
