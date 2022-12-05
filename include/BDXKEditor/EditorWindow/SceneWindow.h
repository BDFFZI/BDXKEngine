#pragma once
#include "BDXKEditor/EditorWindow/Core/EditorWindow.h"
#include "BDXKEngine/Framework/Behavior/Core/Behavior.h"
#include "BDXKEngine/Framework/Renderer/Core/Camera.h"

namespace BDXKEditor
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
