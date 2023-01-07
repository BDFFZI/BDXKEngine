#pragma once
#include "EditorWindow/ConsoleWindow.h"
#include "EditorWindow/GameWindow.h"
#include "EditorWindow/HierarchyWindow.h"
#include "EditorWindow/InspectorWindow.h"
#include "EditorWindow/ProfilerWindow.h"
#include "EditorWindow/ProjectWindow.h"
#include "EditorWindow/SceneWindow.h"
#include "Menu/Core/Menu.h"


namespace BDXKEditor
{
    using namespace BDXKEngine;

    class EditorSystem : public ScriptableObject, public DrawGUIHandler
    {
    public:
        static void Initialize(const std::string& sceneName);

        static const ObjectPtr<SceneWindow>& GetSceneView();
        static const ObjectPtr<HierarchyWindow>& GetHierarchyView();
        static const ObjectPtr<InspectorWindow>& GetInspectorView();
    private:
        static ObjectPtr<EditorSystem> editorSystem;
        static ObjectPtr<SceneWindow> sceneWindow;
        static ObjectPtr<HierarchyWindow> hierarchyWindow;
        static ObjectPtr<InspectorWindow> inspectorWindow;
        static ObjectPtr<ProfilerWindow> profilerWindow;
        static ObjectPtr<ConsoleWindow> consoleWindow;
        static ObjectPtr<ProjectWindow> projectWindow;
        static ObjectPtr<GameWindow> gameWindow;
        static bool isRunning;
        static ObjectPtrBase copying;

        static void SaveScene(); //使用ProjectWindow提供的保存是没有用的，因为它
        CustomMenu("File/Save", SaveScene)
        static void Copy();
        static void Paste();
        static void Play();
        CustomMenu("Edit/Copy", Copy)
        CustomMenu("Edit/Paste", Paste)
        CustomMenuSeparator("Edit/");
        CustomMenu("Edit/Play", Play)

        void DrawMenu(const std::vector<std::string>& path, const std::function<void()>& func, size_t layer = 0);

        void OnDrawGUI() override;
    };

    CustomReflection(EditorSystem)

    void Run(const std::string& sceneName);
}
