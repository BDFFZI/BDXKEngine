#pragma once
#include "Framework/Component.h"
#include "BehaviorEvent.h"

namespace BDXKEngine {
	class Behavior :public Component
	{
		StartHandler* startHandler = nullptr;
		UpdateHandler* updateHandler = nullptr;
		LateUpdateHandler* lateUpdateHandler = nullptr;

		void Awake()override;
		void Enable() override;
		void Disable() override;
	};
}


