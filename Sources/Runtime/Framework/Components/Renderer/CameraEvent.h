#pragma once
#include <vector>
#include "Platform/Window/Window.h"
#include "Platform/GL/GL2D.h"

namespace BDXKEngine {
	class RendererManager;
	class PreRenderHandler {
		friend RendererManager;
	protected:
		virtual void OnPreRender() = 0;
	};

	class PostRenderHandler {
		friend RendererManager;
	protected:
		virtual void OnPostRender() = 0;
	};
}

