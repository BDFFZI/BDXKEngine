#pragma once
#include "Ball.h"
#include "BDXKEngine/Framework/Behavior/Core/Behavior.h"

namespace HelloGame
{
    using namespace BDXKEngine;

    class Bar : public Behavior, public UpdateHandler
    {
        ObjectPtr<GameObject> sphereBar = nullptr;
        ObjectPtr<GameObject> ballPrefab = nullptr;
        int maxBallCount = 1;
        void Transfer(Transferer& transferer) override;
        
        ObjectPtr<GameObject> currentBall = nullptr;
        int currentBallCount = 0;

        void OnUpdate() override;
    };

    CustomReflection(Bar)
}
