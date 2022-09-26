#include "Behavior.h"
#include "BehaviorManager.h"

namespace BDXKEngine
{
	void Behavior::Awake()
	{
		Component::Awake();

		startHandler = dynamic_cast<StartHandler*>(this);
		updateHandler = dynamic_cast<UpdateHandler*>(this);
		lateUpdateHandler = dynamic_cast<LateUpdateHandler*>(this);
	}

	void Behavior::Enable() {
		Component::Enable();
		
		if (startHandler != nullptr) {
			BehaviorManager::allStartHandlers[startHandler] = true;
			startHandler = nullptr;
		}
		if (updateHandler != nullptr) BehaviorManager::allUpdateHandlers[updateHandler] = true;
		if (lateUpdateHandler != nullptr) BehaviorManager::allLateUpdateHandlers[lateUpdateHandler] = true;

		if (const auto handler = dynamic_cast<EnableHandler*>(this); handler != nullptr)
			handler->OnEnable();
	}

	void Behavior::Disable() {
		if (const auto handler = dynamic_cast<DisableHandler*>(this); handler != nullptr)
			handler->OnDisable();
		
		if (updateHandler != nullptr) BehaviorManager::allUpdateHandlers[updateHandler] = false;
		if (lateUpdateHandler != nullptr) BehaviorManager::allLateUpdateHandlers[lateUpdateHandler] = false;
		
		Component::Disable();
	}
}
