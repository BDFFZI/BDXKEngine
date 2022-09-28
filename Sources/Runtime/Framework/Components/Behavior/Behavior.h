#pragma once
#include "Framework/Component.h"
#include "BehaviorEvent.h"

namespace BDXKEngine
{
    class Behavior : public Component
    {
        StartHandler* startHandler = nullptr;
        UpdateHandler* updateHandler = nullptr;
        LateUpdateHandler* lateUpdateHandler = nullptr;
        bool isAwakened = false;

        void Awake() override;
        void Destroy() override;
        void Enable() override;
        void Disable() override;
    };
}
