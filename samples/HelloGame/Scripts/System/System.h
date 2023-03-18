#pragma once
#include "BDXKEngine/Framework/Behavior/Core/Behavior.h"

namespace HelloGame
{
    using namespace BDXKEngine;

    class System : public Behavior, public AwakeHandler
    {
    public:
        void OnAwake() override;
    };

    CustomReflection(System)
}
