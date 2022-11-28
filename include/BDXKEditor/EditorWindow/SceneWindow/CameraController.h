#pragma once
#include "BDXKEngine/Framework/Behavior/Core/Behavior.h"
#include "BDXKEngine/Framework/Renderer/Core/Camera.h"

namespace BDXKEngine
{
    class CameraController : public Behavior, public StartHandler, public UpdateHandler
    {
    public:
        bool IsControlling() const;
    private:
        ObjectPtr<GameObject> transform = {};
        bool isControlling = false;

        void OnStart() override;
        void OnUpdate() override;
    };
    CustomReflection(CameraController)
}
