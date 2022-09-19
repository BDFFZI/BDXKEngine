#include "Behavior.h"
#include "BehaviorManager.h"

void BDXKEngine::Behavior::Awake()
{
	Component::Awake();

	StartHandler* startHandler = dynamic_cast<StartHandler*>(this);
	UpdateHandler* updateHandler = dynamic_cast<UpdateHandler*>(this);
	LateUpdateHandler* lateUpdateHandler = dynamic_cast<LateUpdateHandler*>(this);

	if (startHandler != nullptr) BehaviorManager::allStartHandlers.push_back(startHandler);
	if (updateHandler != nullptr) BehaviorManager::allUpdateHandlers.push_back(updateHandler);
	if (lateUpdateHandler != nullptr) BehaviorManager::allLateUpdateHandlers.push_back(lateUpdateHandler);
}

void BDXKEngine::Behavior::Destroy()
{
	StartHandler* startHandler = dynamic_cast<StartHandler*>(this);
	UpdateHandler* updateHandler = dynamic_cast<UpdateHandler*>(this);
	LateUpdateHandler* lateUpdateHandler = dynamic_cast<LateUpdateHandler*>(this);

	if (startHandler != nullptr) BehaviorManager::invalidStartHandlers.insert(startHandler);
	if (updateHandler != nullptr) BehaviorManager::invalidUpdateHandlers.insert(updateHandler);
	if (lateUpdateHandler != nullptr) BehaviorManager::invalidLateUpdateHandlers.insert(lateUpdateHandler);

	Component::Destroy();
}
