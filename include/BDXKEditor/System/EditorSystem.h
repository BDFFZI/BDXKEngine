#pragma once
#include "BDXKEngine/Framework/Component/Behavior/Behavior.h"
#include "BDXKEngine/Framework/Component/Renderer/RenderEvent.h"
#include "BDXKEngine/Framework/Scene/Scene.h"

namespace BDXKEditor
{
    class InspectorView;
    class HierarchyView;
    class SceneView;
    using namespace BDXKEngine;

    class EditorSystem : public Behavior, public AwakeHandler,public DestroyHandler, public DrawGUIHandler
    {
    public:
        static ObjectPtr<Scene> mainScene;
        static ObjectPtr<SceneView> sceneView;
        static ObjectPtr<HierarchyView> hierarchyView;
        static ObjectPtr<InspectorView> inspectorView;
        static ObjectPtrBase dragging;
        static ObjectPtrBase focusing;
        
        void OnDrawGUI() override;
        void OnAwake() override;
        void OnDestroy() override;
    };
}
