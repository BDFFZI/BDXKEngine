#pragma once
#include "BDXKEngine/Framework/Component/Component.h"
#include "BehaviorEvent.h"

namespace BDXKEngine
{
    class Behavior : public Component
    {
        StartHandler* startHandler = nullptr;
        UpdateHandler* updateHandler = nullptr;
        LateUpdateHandler* lateUpdateHandler = nullptr;
        
        void MarkAwake() override;
        void Enable() override;
        void Disable() override;
    };
}
