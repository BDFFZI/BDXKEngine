#pragma once
#include "BDXKEditor/EditorWindow/Core/EditorWindow.h"
#include "BDXKEngine/Framework/Behavior/Core/Behavior.h"
#include "BDXKEngine/Framework/Renderer/Core/Camera.h"
#include "BDXKEngine/Platform/GUI/GUI.h"
#include <ImGuizmo/ImGuizmo.h>

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
        ObjectPtr<GameObject> gameObject;
        ObjectPtr<Camera> editorCamera;
        ObjectPtr<CameraController> cameraController;

        ObjectPtr<Texture2D> cameraTexture;
        Vector2 viewSize;
        int cameraOption = 0;
        int handleOption = 1;
        bool HasMenu() override;

        void OnAwake() override;
        void OnGUI() override;
    };
}
