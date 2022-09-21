#include "BehaviorManager.h"
#include "Behavior.h"

namespace BDXKEngine {
	std::unordered_map<StartHandler*, bool> BehaviorManager::allStartHandlers = {};
	std::unordered_map<UpdateHandler*, bool> BehaviorManager::allUpdateHandlers = {};
	std::unordered_map<LateUpdateHandler*, bool> BehaviorManager::allLateUpdateHandlers = {};

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
		std::unordered_map<StartHandler*, bool> allStartHandlers = { BehaviorManager::allStartHandlers };
		for (auto handlerPair = allStartHandlers.begin(); handlerPair != allStartHandlers.end(); handlerPair++)
		{
			auto handler = handlerPair->first;
			if (BehaviorManager::allStartHandlers[handler] == true)
				handler->OnStart();
			BehaviorManager::allStartHandlers.erase(handler);
		}
	}
	void BehaviorManager::Update()
	{
		Start();
		std::unordered_map<UpdateHandler*, bool> allUpdateHandlers = { BehaviorManager::allUpdateHandlers };
		for (auto handlerPair = allUpdateHandlers.begin(); handlerPair != allUpdateHandlers.end(); handlerPair++)
		{
			auto handler = handlerPair->first;
			if (BehaviorManager::allUpdateHandlers[handler] == true)
				handler->OnUpdate();
			else
				BehaviorManager::allUpdateHandlers.erase(handler);
		}
	}
	void BehaviorManager::LateUpdate()
	{
		Start();
		std::unordered_map<LateUpdateHandler*, bool> allLateUpdateHandlers = { BehaviorManager::allLateUpdateHandlers };
		for (auto handlerPair = allLateUpdateHandlers.begin(); handlerPair != allLateUpdateHandlers.end(); handlerPair++)
		{
			auto handler = handlerPair->first;
			if (BehaviorManager::allLateUpdateHandlers[handler] == true)
				handler->OnLateUpdate();
			else
				BehaviorManager::allLateUpdateHandlers.erase(handler);
		}
	}
}