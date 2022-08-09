#pragma once
#include <vector>
#include "Window.h"
#include "GL2D.h"

namespace BDXKEngine {
	class RendererManager :GL
	{
	public:
		static RendererManager* Initialize(Window* window);
	};

	class RenderObjectHandler {
		friend RendererManager;
	protected:
		RenderObjectHandler();
		virtual void OnRenderObject() = 0;
	private:

	};

	class DrawGizmosHandler {
		friend RendererManager;
	protected:
		DrawGizmosHandler();
		virtual void OnDrawGizmos() = 0;
	private:

	};


}

