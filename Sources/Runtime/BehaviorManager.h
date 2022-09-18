#pragma once
#include <unordered_set>
#include "Window.h"
#include "BehaviorEvent.h"

namespace BDXKEngine {
	class Behavior;
	class BehaviorManager
	{
		friend Behavior;
	public:
		static BehaviorManager* Initialize(Window* window);
	private:
		static void Start();
		static void Update();
		static void LateUpdate();

		static std::vector<StartHandler*> allStartHandlers;
		static std::vector<UpdateHandler*> allUpdateHandlers;
		static std::vector<LateUpdateHandler*> allLateUpdateHandlers;
		static std::unordered_set<void*> invalidHandlers;//标记指针是否失效
	};

}

