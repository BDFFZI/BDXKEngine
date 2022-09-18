#pragma once
#include <vector>
#include "Window.h"
#include "GL2D.h"

namespace BDXKEngine {
	class RendererManager;
	class RenderObjectHandler {
		friend RendererManager;
	protected:
		virtual void OnRenderObject() = 0;
	};

	class DrawGizmosHandler {
		friend RendererManager;
	protected:
		virtual void OnDrawGizmos() = 0;
	};
}

