#pragma once
#include <unordered_map>
#include "Platform/Window/Window.h"
#include "BehaviorEvent.h"
#include "Base/Extension/Debug.h"

namespace BDXKEngine {
	class Behavior;
	class BehaviorManager
	{
		friend Behavior;
	public:
		static void DebugHandlersCount()
		{
			std::wstringstream stream = {};
			stream << L"StartHandler数量:" << std::to_wstring(allStartHandlers.size()) << std::endl;
			stream << L"UpdateHandler数量:" << std::to_wstring(allUpdateHandlers.size()) << std::endl;
			stream << L"LateUpdateHandler数量:" << std::to_wstring(allLateUpdateHandlers.size());
			Debug::LogWarning(stream.str());
		}

		static BehaviorManager* Initialize(Window* window);
	private:
		static void Start();
		static void Update();
		static void LateUpdate();
		
		//由于删除物体的功能，会导致遍历容器时出现野指针的情况，为保证遍历继续正常运行，需要延迟处理
		//通过invalid标记告知BehaviorManager，以便等到安全状态时再统一处理
		//添加时也很被延迟处理
		static std::unordered_map<StartHandler*, bool> allStartHandlers;
		static std::unordered_map<UpdateHandler*, bool> allUpdateHandlers;
		static std::unordered_map<LateUpdateHandler*, bool> allLateUpdateHandlers;
	};

}

