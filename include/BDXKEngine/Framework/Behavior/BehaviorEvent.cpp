#include "BehaviorEvent.h"
#include "Core/Behavior.h"

namespace BDXKEngine
{
    void BehaviorEvent::Initialize(Window* window)
    {
        window->AddRenewEvent([]()
        {
            Behavior::Update();
            Behavior::LateUpdate();
        });
    }
}
