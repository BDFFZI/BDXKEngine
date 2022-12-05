#pragma once
#include "EditorWindow/ConsoleWindow.h"
#include "EditorWindow/HierarchyWindow.h"
#include "EditorWindow/InspectorWindow.h"
#include "EditorWindow/ProfilerWindow.h"
#include "EditorWindow/ProjectWindow.h"
#include "EditorWindow/SceneWindow.h"


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
        static ObjectPtr<ProjectWindow> projectWindow;

        std::string sceneFile;

        void OnDrawGUI() override;
        void OnAwake() override;
    };

    CustomReflection(EditorSystem)

    void TestLight();
    void Run(const std::string& sceneFile);
}
