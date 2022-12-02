#pragma once
#include "EditorWindow/ConsoleWindow/ConsoleWindow.h"
#include "EditorWindow/HierarchyWindow/HierarchyWindow.h"
#include "EditorWindow/InspectorWindow/InspectorWindow.h"
#include "EditorWindow/ProfilerWindow/ProfilerWindow.h"
#include "EditorWindow/SceneWindow/SceneWindow.h"


namespace BDXKEditor
{
    using namespace BDXKEngine;

    class EditorSystem : public ScriptableObject, public AwakeHandler, public DrawGUIHandler
    {
    public:
        static const ObjectPtr<SceneWindow>& GetSceneView();
        static const ObjectPtr<HierarchyWindow>& GetHierarchyView();
        static const ObjectPtr<InspectorWindow>& GetInspectorView();

        void SetScene(const std::string& name);
    private:
        static ObjectPtr<SceneWindow> sceneWindow;
        static ObjectPtr<HierarchyWindow> hierarchyWindow;
        static ObjectPtr<InspectorWindow> inspectorWindow;
        static ObjectPtr<ProfilerWindow> profilerWindow;
        static ObjectPtr<ConsoleWindow> consoleWindow;

        std::string sceneFile;

        void OnDrawGUI() override;
        void OnAwake() override;
    };

    CustomReflection(EditorSystem)

    void TestLight();
    void Run(const std::string& sceneFile);
}
