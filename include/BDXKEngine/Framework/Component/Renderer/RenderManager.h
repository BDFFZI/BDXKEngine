#pragma once
#include <vector>
#include "Renderer.h"
#include "BDXKEngine/Platform/Window/Window.h"
#include "BDXKEngine/Framework/Component/Renderer/Camera.h"

namespace BDXKEngine {
	class RenderManager :protected GL
	{
	public:
		static RenderManager* Initialize(Window* window);
		static std::vector<ObjectPtr<Renderer>> GetRenderersQueue();
		static std::vector<ObjectPtr<Camera>> GetCamerasQueue();
		static std::vector<ObjectPtr<Renderer>> renderers;
	};
}
