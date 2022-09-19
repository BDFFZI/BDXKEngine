#include "Behavior.h"
#include "BehaviorManager.h"

void BDXKEngine::Behavior::SetEnabling(bool state)
{
	if (isEnabling != state && IsActivating())
	{
		if (state)
			Enable();
		else
			Disable();
	}

	isEnabling = state;
}

void BDXKEngine::Behavior::Awake()
{
	Component::Awake();

	startHandler = dynamic_cast<StartHandler*>(this);
	updateHandler = dynamic_cast<UpdateHandler*>(this);
	lateUpdateHandler = dynamic_cast<LateUpdateHandler*>(this);

	if (isEnabling) Enable();
}

void BDXKEngine::Behavior::Destroy()
{
	if (isEnabling) Disable();

	Component::Destroy();
}

void BDXKEngine::Behavior::Enable() {
	if (startHandler != nullptr) {
		BehaviorManager::allStartHandlers[startHandler] = true;
		startHandler = nullptr;
	}

	if (updateHandler != nullptr) BehaviorManager::allUpdateHandlers[updateHandler] = true;
	if (lateUpdateHandler != nullptr) BehaviorManager::allLateUpdateHandlers[lateUpdateHandler] = true;
}

void BDXKEngine::Behavior::Disable() {
	if (updateHandler != nullptr) BehaviorManager::allUpdateHandlers[updateHandler] = false;
	if (lateUpdateHandler != nullptr) BehaviorManager::allLateUpdateHandlers[lateUpdateHandler] = false;
}
