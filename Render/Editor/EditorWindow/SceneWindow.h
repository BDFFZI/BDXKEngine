#pragma once
#include "Core/Editor/Core/Editor/EditorWindow.h"
#include "Core/Runtime/Function/GUI/GUI.h"
#include "Render/Runtime/Renderer/Core/Camera.h"

namespace BDXKEditor
{
    class CameraController : public Behavior, public StartEvent, public UpdateEvent
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

    class SceneWindow : public EditorWindow, public AwakeEvent
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

    CustomReflection(SceneWindow)
}
