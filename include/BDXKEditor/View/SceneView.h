#pragma once
#include <BDXKEngine/Engine.h>
#include "EditorWindow.h"

namespace BDXKEditor
{
    using namespace BDXKEngine;

    class SceneView : public EditorWindow, public AwakeHandler, public PreRenderFrameHandler, TransformManager, GL
    {
    public:
        ObjectPtr<Camera> GetEditorCamera() const;
    private:
        ObjectPtr<Camera> editorCamera;
        ObjectPtr<Texture2D> editorCameraTexture;
        Vector2 viewSize;

        void OnAwake() override;
        void OnPreRenderFrame() override;
        void OnGUI() override;
    };
}
