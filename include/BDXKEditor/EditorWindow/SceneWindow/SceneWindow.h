#pragma once
#include "CameraController.h"
#include "BDXKEditor/EditorWindow/Core/EditorWindow.h"
#include "BDXKEngine/Framework/Renderer/Core/Camera.h"

namespace BDXKEditor
{
    using namespace BDXKEngine;

    class SceneWindow : public EditorWindow, public AwakeHandler
    {
    public:
        void SetTarget(const ObjectPtr<GameObject>& target);
    private:
        ObjectPtr<GameObject> target;
        ObjectPtr<Camera> camera;
        ObjectPtr<Texture2D> cameraTexture;
        ObjectPtr<CameraController> cameraController;
        Vector2 viewSize;

        void OnAwake() override;
        void OnGUI() override;
    };
}
