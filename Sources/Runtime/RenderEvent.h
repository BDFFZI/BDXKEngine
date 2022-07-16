#pragma once
#include <vector>

namespace BDXKEngine {

	class RenderEventEditor;

	class RenderObjectEvent {
		friend RenderEventEditor;
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
		friend RenderEventEditor;
	protected:
		DrawGizmosEvent()
		{
			drawGizmosEvents.push_back(this);
		}

		virtual void OnDrawGizmos() = 0;
	private:
		static std::vector<DrawGizmosEvent*> drawGizmosEvents;
	};

	class RenderEventEditor {
	protected:
		static void RenderObject(RenderObjectEvent* eventP)
		{
			eventP->OnRenderObject();
		}
		static void DrawGizmos(DrawGizmosEvent* eventP)
		{
			eventP->OnDrawGizmos();
		}

		static std::vector<RenderObjectEvent*>& GetRenderObjectEvents()
		{
			return RenderObjectEvent::renderObjectEvents;
		}
		static std::vector<DrawGizmosEvent*>& GetDrawGizmosEvents()
		{
			return DrawGizmosEvent::drawGizmosEvents;
		}
	};
}