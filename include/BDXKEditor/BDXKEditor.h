#pragma once
#include "EditorWindow/HierarchyWindow/HierarchyWindow.h"
#include "EditorWindow/InspectorWindow/InspectorWindow.h"
#include "EditorWindow/SceneWindow/SceneWindow.h"


namespace BDXKEditor
{
    using namespace BDXKEngine;

    class EditorSystem : public Component, public AwakeHandler, public DestroyHandler, public DrawGUIHandler
    {
    public:
        static const ObjectPtr<SceneWindow>& GetSceneView();
        static const ObjectPtr<HierarchyWindow>& GetHierarchyView();
        static const ObjectPtr<InspectorWindow>& GetInspectorView();
    private:
        static ObjectPtr<SceneWindow> sceneView;
        static ObjectPtr<HierarchyWindow> hierarchyView;
        static ObjectPtr<InspectorWindow> inspectorView;


        void OnDrawGUI() override;
        void OnAwake() override;
        void OnDestroy() override;
    };
    CustomReflection(EditorSystem)

    void TestLight();
    void Run();
}
