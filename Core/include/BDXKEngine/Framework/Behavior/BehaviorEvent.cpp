#include "BehaviorEvent.h"

#include "BDXKEngine/Platform/Window/Window.h"
#include "Core/Behavior.h"

namespace BDXKEngine
{
    void BehaviorEvent::Initialize()
    {
        Window::AddRenewEvent([]()
        {
            Behavior::Update();
            Behavior::LateUpdate();
        });
    }
}
