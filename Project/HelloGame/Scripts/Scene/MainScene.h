#pragma once
#include "BDXKEngine/Framework/Behavior/Core/Behavior.h"
#include "BDXKEngine/Framework/Renderer/RenderEvent.h"

namespace HelloGame
{
    using namespace BDXKEngine;

    class MainScene : public Behavior, public DrawGUIHandler
    {
        void OnDrawGUI() override;
    };

    CustomReflection(MainScene)
}
