#pragma once
#include <BDXKEngine/Engine.h>
#include "EditorWindow.h"
#include "BDXKEditor/System/EditorSystem.h"

namespace BDXKEditor
{
    using namespace BDXKEngine;

    class SceneView : public EditorWindow, public PreRenderFrameHandler, TransformManager, GL
    {
    public:
        ObjectPtr<Camera> GetEditorCamera() const;
    private:
        ObjectPtr<Camera> editorCamera;
        ObjectPtr<Texture2D> editorCameraTexture;
        Vector2 viewSize;

        void OnPreRenderFrame() override;
        void OnDrawWindow() override;
        void OnShow() override;
    };
}
