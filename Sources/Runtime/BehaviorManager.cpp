#include "BehaviorManager.h"
#include "BehaviorEvent.h"

namespace BDXKEngine {
	std::vector<StartHandler*> BehaviorManager::allStartHandlers = {};
	std::vector<UpdateHandler*> BehaviorManager::allUpdateHandlers = {};
	std::vector<LateUpdateHandler*> BehaviorManager::allLateUpdateHandlers = {};
	std::unordered_set<StartHandler*> BehaviorManager::invalidStartHandlers = {};//标记指针是否失效
	std::unordered_set<UpdateHandler*> BehaviorManager::invalidUpdateHandlers = {};//标记指针是否失效
	std::unordered_set<LateUpdateHandler*> BehaviorManager::invalidLateUpdateHandlers = {};//标记指针是否失效

	BehaviorManager* BehaviorManager::Initialize(Window* window)
	{
		window->AddRenewEvent([]() {
			Update();
			LateUpdate();
			});
		return nullptr;
	}
	void BehaviorManager::Start()
	{
		for (auto handlerPtr = allStartHandlers.begin(); handlerPtr != allStartHandlers.end();)
		{
			auto handler = *handlerPtr;
			if (invalidStartHandlers.contains(handler) == false)
			{
				handler->OnStart();
				handlerPtr++;
			}
			else
			{
				handlerPtr = allStartHandlers.erase(handlerPtr);
				invalidStartHandlers.erase(handler);
			}
		}

		allStartHandlers.clear();
	}
	void BehaviorManager::Update()
	{
		Start();
		for (auto handlerPtr = allUpdateHandlers.begin(); handlerPtr != allUpdateHandlers.end();)
		{
			auto handler = *handlerPtr;
			if (invalidUpdateHandlers.contains(handler) == false)
			{
				handler->OnUpdate();
				handlerPtr++;
			}
			else
			{
				handlerPtr = allUpdateHandlers.erase(handlerPtr);
				invalidUpdateHandlers.erase(handler);
			}
		}
	}
	void BehaviorManager::LateUpdate()
	{
		Start();
		for (auto handlerPtr = allLateUpdateHandlers.begin(); handlerPtr != allLateUpdateHandlers.end();)
		{
			auto handler = *handlerPtr;
			if (invalidLateUpdateHandlers.contains(handler) == false)
			{
				handler->OnLateUpdate();
				handlerPtr++;
			}
			else
			{
				handlerPtr = allLateUpdateHandlers.erase(handlerPtr);
				invalidLateUpdateHandlers.erase(handler);
			}
		}
	}
}