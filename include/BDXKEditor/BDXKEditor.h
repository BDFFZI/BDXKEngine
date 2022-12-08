#pragma once
#include "BDXKEngine/Framework/Scene.h"
#include "EditorWindow/ConsoleWindow.h"
#include "EditorWindow/GameWindow.h"
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

        void SetScene(const std::string& sceneName);
    private:
        static ObjectPtr<SceneWindow> sceneWindow;
        static ObjectPtr<HierarchyWindow> hierarchyWindow;
        static ObjectPtr<InspectorWindow> inspectorWindow;
        static ObjectPtr<ProfilerWindow> profilerWindow;
        static ObjectPtr<ConsoleWindow> consoleWindow;
        static ObjectPtr<ProjectWindow> projectWindow;
        static ObjectPtr<GameWindow> gameWindow;

        std::string sceneName;
        bool isRunning;

        void OnDrawGUI() override;
        void DrawGUI();
        void OnAwake() override;
    };

    CustomReflection(EditorSystem)

    void TestLight();
    void Run(const std::string& sceneName);
}
