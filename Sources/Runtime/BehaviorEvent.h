#pragma once
#include "Window.h"

namespace BDXKEngine {
	class BehaviorManager
	{
	public:
		static BehaviorManager* Initialize(Window* window);
	protected:
		static void Start();
		static void Update();
		static void LateUpdate();
	};

	class StartHandler {
		friend BehaviorManager;
	protected:
		StartHandler();
		~StartHandler();
		virtual void OnStart() = 0;
	};
	class UpdateHandler {
		friend BehaviorManager;
	protected:
		UpdateHandler();
		~UpdateHandler();
		virtual void OnUpdate() = 0;
	};
	class LateUpdateHandler {
		friend BehaviorManager;
	protected:
		LateUpdateHandler();
		~LateUpdateHandler();
		virtual void OnLateUpdate() = 0;
	};
}
