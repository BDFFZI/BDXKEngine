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
		//由于删除物体的功能，会导致遍历容器时出现元素无效的情况，未保证遍历继续正常运行，需要延迟处理
		//通过invalid标记告知BehaviorManager，以便等到安全状态时再统一处理
		static std::unordered_set<StartHandler*> invalidStartHandlers;
		static std::unordered_set<UpdateHandler*> invalidUpdateHandlers;
		static std::unordered_set<LateUpdateHandler*> invalidLateUpdateHandlers;
	};

}

