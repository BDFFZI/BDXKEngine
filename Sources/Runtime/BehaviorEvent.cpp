#include "BehaviorEvent.h"
#include <unordered_set>
namespace BDXKEngine {
	inline std::vector<StartHandler*> allStartEvents;
	inline std::vector<UpdateHandler*> allUpdateEvents;
	inline std::vector<LateUpdateHandler*> allLateUpdateEvents;
	inline std::unordered_set<void*> behaviorStates;//标记指针是否失效

	void BehaviorManager::Start()
	{
		for (auto handlerPtr = allStartEvents.begin(); handlerPtr != allStartEvents.end();)
		{
			auto handler = *handlerPtr;
			if (behaviorStates.contains(handler) == false)
			{
				handler->OnStart();
				handlerPtr++;
			}
			else
			{
				allStartEvents.erase(handlerPtr);
				behaviorStates.erase(handler);
			}
		}

		allStartEvents.clear();
	}
	void BehaviorManager::Update()
	{
		Start();
		for (auto handlerPtr = allUpdateEvents.begin(); handlerPtr != allUpdateEvents.end();)
		{
			auto handler = *handlerPtr;
			if (behaviorStates.contains(handler) == false)
			{
				handler->OnUpdate();
				handlerPtr++;
			}
			else
			{
				allUpdateEvents.erase(handlerPtr);
				behaviorStates.erase(handler);
			}
		}
	}
	void BehaviorManager::LateUpdate()
	{
		Start();
		for (auto handlerPtr = allLateUpdateEvents.begin(); handlerPtr != allLateUpdateEvents.end();)
		{
			auto handler = *handlerPtr;
			if (behaviorStates.contains(handler) == false)
			{
				handler->OnLateUpdate();
				handlerPtr++;
			}
			else
			{
				allLateUpdateEvents.erase(handlerPtr);
				behaviorStates.erase(handler);
			}
		}
	}

	BehaviorManager* BehaviorManager::Initialize(Window* window)
	{
		window->AddRenewEvent([]() {
			Update();
			LateUpdate();
			});
		return nullptr;
	}

	StartHandler::StartHandler()
	{
		allStartEvents.push_back(this);
	}
	StartHandler::~StartHandler()
	{
		behaviorStates.insert(this);
	}
	UpdateHandler::UpdateHandler()
	{
		allUpdateEvents.push_back(this);
	}
	UpdateHandler::~UpdateHandler()
	{
		behaviorStates.insert(this);
	}
	LateUpdateHandler::LateUpdateHandler()
	{
		allLateUpdateEvents.push_back(this);
	}
	LateUpdateHandler::~LateUpdateHandler()
	{
		behaviorStates.insert(this);
	}
}

