#include "Behavior.h"
#include "BehaviorManager.h"

namespace BDXKEngine
{
    void Behavior::MarkAwake()
    {
        startHandler = dynamic_cast<StartHandler*>(this);
        updateHandler = dynamic_cast<UpdateHandler*>(this);
        lateUpdateHandler = dynamic_cast<LateUpdateHandler*>(this);

        Component::MarkAwake();
    }

    void Behavior::Enable()
    {
        Component::Enable();

        if (startHandler != nullptr)
        {
            BehaviorManager::allStartHandlers[startHandler] = true;
            startHandler = nullptr;
        }
        if (updateHandler != nullptr) BehaviorManager::allUpdateHandlers[updateHandler] = true;
        if (lateUpdateHandler != nullptr) BehaviorManager::allLateUpdateHandlers[lateUpdateHandler] = true;
    }

    void Behavior::Disable()
    {
        if (updateHandler != nullptr) BehaviorManager::allUpdateHandlers[updateHandler] = false;
        if (lateUpdateHandler != nullptr) BehaviorManager::allLateUpdateHandlers[lateUpdateHandler] = false;

        Component::Disable();
    }
}
