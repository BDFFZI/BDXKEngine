#pragma once
#include <vector>

namespace BDXKEngine {

	class Graphics;

	class RenderObjectEvent {
		friend Graphics;
	protected:
		RenderObjectEvent()
		{
			renderObjectEvents.push_back(this);
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
			drawGizmosEvents.push_back(this);
		}

		virtual void OnDrawGizmos() = 0;
	private:
		static std::vector<DrawGizmosEvent*> drawGizmosEvents;
	};
}