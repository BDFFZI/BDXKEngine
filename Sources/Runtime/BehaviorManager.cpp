#include "BehaviorManager.h"
#include "BehaviorEvent.h"

namespace BDXKEngine {
	std::vector<StartHandler*> BehaviorManager::allStartHandlers = {};
	std::vector<UpdateHandler*> BehaviorManager::allUpdateHandlers = {};
	std::vector<LateUpdateHandler*> BehaviorManager::allLateUpdateHandlers = {};
	std::unordered_set<void*> BehaviorManager::invalidHandlers = {};//标记指针是否失效

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
			if (invalidHandlers.contains(handler) == false)
			{
				handler->OnStart();
				handlerPtr++;
			}
			else
			{
				allStartHandlers.erase(handlerPtr);
				invalidHandlers.erase(handler);
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
			if (invalidHandlers.contains(handler) == false)
			{
				handler->OnUpdate();
				handlerPtr++;
			}
			else
			{
				allUpdateHandlers.erase(handlerPtr);
				invalidHandlers.erase(handler);
			}
		}
	}
	void BehaviorManager::LateUpdate()
	{
		Start();
		for (auto handlerPtr = allLateUpdateHandlers.begin(); handlerPtr != allLateUpdateHandlers.end();)
		{
			auto handler = *handlerPtr;
			if (invalidHandlers.contains(handler) == false)
			{
				handler->OnLateUpdate();
				handlerPtr++;
			}
			else
			{
				allLateUpdateHandlers.erase(handlerPtr);
				invalidHandlers.erase(handler);
			}
		}
	}
}