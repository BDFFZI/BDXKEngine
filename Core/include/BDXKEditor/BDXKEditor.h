#pragma once
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
        static void LoadScene(const std::string& sceneName,bool keepPersistent = true);
        static void SaveScene(const std::string& sceneName);

        std::string sceneName;
        bool isRunning = false;
        
        void DrawMainMenuBar();
        void OnDrawGUI() override;
        void OnAwake() override;
    };

    CustomReflection(EditorSystem)

    void Run(const std::string& sceneName);
}
