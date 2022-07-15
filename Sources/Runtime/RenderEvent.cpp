#include "RenderEvent.h"

namespace BDXKEngine {
	std::vector<RenderObjectEvent*> RenderObjectEvent::renderObjectEvents = {};
	std::vector<DrawGizmosEvent*> DrawGizmosEvent::drawGizmosEvents = {};
}