#pragma once
#include "Platform/Window/Window.h"

namespace BDXKEngine {
	class BehaviorManager;
	class StartHandler {
		friend BehaviorManager;
	protected:
		virtual void OnStart() = 0;
	};
	class UpdateHandler {
		friend BehaviorManager;
	protected:
		virtual void OnUpdate() = 0;
	};
	class LateUpdateHandler {
		friend BehaviorManager;
	protected:
		virtual void OnLateUpdate() = 0;
	};
}
