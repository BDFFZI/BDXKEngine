#pragma once
#include <vector>
#include "Platform/Window/Window.h"
#include "Platform/GL/GL2D.h"
#include "Framework/Components/Renderer/CameraEvent.h"

namespace BDXKEngine {
	class Renderer;
	class RendererManager :protected GL
	{
		friend Renderer;
	public:
		static RendererManager* Initialize(Window* window);
	protected:
		static std::vector<ObjectPtr<Renderer>> GetRenderersQueue();
	private:
		static std::vector<ObjectPtr<Renderer>> renderers;
	};
}
