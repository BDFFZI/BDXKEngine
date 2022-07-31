#pragma once
#include <vector>

namespace BDXKEngine {

	class Graphics;

	class RenderObjectEvent {
		friend Graphics;
	protected:
		RenderObjectEvent()
		{
			renderObjectEvents.insert(renderObjectEvents.begin(), this);
		}

		virtual void OnRenderObject() = 0;
	private:
		static std::vector<RenderObjectEvent*> renderObjectEvents;
	};

	class DrawGizmosEvent {
		friend Graphics;
	protected:
		DrawGizmosEvent()
		{
			drawGizmosEvents.insert(drawGizmosEvents.begin(), this);
		}

		virtual void OnDrawGizmos() = 0;
	private:
		static std::vector<DrawGizmosEvent*> drawGizmosEvents;
	};
}