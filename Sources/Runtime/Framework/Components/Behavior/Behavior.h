#pragma once
#include "Framework/Component.h"
#include "BehaviorEvent.h"

namespace BDXKEngine {
	class Behavior :public Component
	{
	public:
		void SetEnabling(bool state);
	protected:
		void Awake()override;
		void Destroy()override;
		virtual void Enable();
		virtual void Disable();
	private:
		bool isEnabling = true;
		StartHandler* startHandler;
		UpdateHandler* updateHandler;
		LateUpdateHandler* lateUpdateHandler;
	};
}


