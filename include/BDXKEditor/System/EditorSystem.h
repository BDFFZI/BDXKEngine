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

    class EditorSystem : public Behavior, public AwakeHandler, public DestroyHandler, public DrawGUIHandler
    {
    public:
        static const ObjectPtr<Scene>& GetMainScene();
        static const ObjectPtrBase& GetDragging();
        static const ObjectPtrBase& GetFocusing();
        static const ObjectPtr<SceneView>& GetSceneView();
        static const ObjectPtr<HierarchyView>& GetHierarchyView();
        static const ObjectPtr<InspectorView>& GetInspectorView();

        static void SetMainScene(const ObjectPtr<Scene>& mainScene);
        static void SetDragging(const ObjectPtrBase& dragging);
        static void SetFocusing(const ObjectPtrBase& focusing);
    private:
        static ObjectPtr<Scene> mainScene;
        static ObjectPtrBase dragging;
        static ObjectPtrBase focusing;
        static ObjectPtr<SceneView> sceneView;
        static ObjectPtr<HierarchyView> hierarchyView;
        static ObjectPtr<InspectorView> inspectorView;

        void OnDrawGUI() override;
        void OnAwake() override;
        void OnDestroy() override;
    };
}
