#pragma once
#include "BDXKEngine/Framework/Behavior/Core/Behavior.h"
#include "BDXKEngine/Framework/Renderer/RenderEvent.h"
#include "BDXKEngine/Framework/Renderer/Core/Camera.h"

namespace BDXKEngine
{
    class CameraController : public Behavior, public StartHandler, public UpdateHandler
    {
        ObjectPtr<GameObject> transform = {};

        void OnStart() override;
        void OnUpdate() override;
    };
}
