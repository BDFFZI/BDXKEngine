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
		const std::unordered_map allStartHandlers = { BehaviorManager::allStartHandlers };
		for (auto& allStartHandler : allStartHandlers)
		{
			auto handler = allStartHandler.first;
			if (BehaviorManager::allStartHandlers[handler] == true)
				handler->OnStart();
			BehaviorManager::allStartHandlers.erase(handler);
		}
	}
	void BehaviorManager::Update()
	{
		Start();
		const std::unordered_map allUpdateHandlers = { BehaviorManager::allUpdateHandlers };
		for (auto& allUpdateHandler : allUpdateHandlers)
		{
			auto handler = allUpdateHandler.first;
			if (BehaviorManager::allUpdateHandlers[handler] == true)
				handler->OnUpdate();
			else
				BehaviorManager::allUpdateHandlers.erase(handler);
		}
	}
	void BehaviorManager::LateUpdate()
	{
		Start();
		const std::unordered_map allLateUpdateHandlers = { BehaviorManager::allLateUpdateHandlers };
		for (auto& allLateUpdateHandler : allLateUpdateHandlers)
		{
			auto handler = allLateUpdateHandler.first;
			if (BehaviorManager::allLateUpdateHandlers[handler] == true)
				handler->OnLateUpdate();
			else
				BehaviorManager::allLateUpdateHandlers.erase(handler);
		}
	}
}