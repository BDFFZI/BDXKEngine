#pragma once
#include "BDXKEngine/BDXKEngine.h"
#include "Framework/EditorWindow/ConsoleWindow.h"
#include "Framework/EditorWindow/GameWindow.h"
#include "Framework/EditorWindow/HierarchyWindow.h"
#include "Framework/EditorWindow/InspectorWindow.h"
#include "Framework/EditorWindow/ProfilerWindow.h"
#include "Framework/EditorWindow/ProjectWindow.h"
#include "Framework/EditorWindow/SceneWindow.h"
#include "Framework/Menu/Core/Menu.h"


namespace BDXKEditor
{
    using namespace BDXKEngine;

    class EditorSystem : public ScriptableObject, public DrawGUIHandler
    {
    public:
        static const ObjectPtr<SceneWindow>& GetSceneView();
        static const ObjectPtr<HierarchyWindow>& GetHierarchyView();
        static const ObjectPtr<InspectorWindow>& GetInspectorView();
    private:
        struct Settings : Reflective
        {
            std::string sceneName;

            void Transfer(Transferer& transferer) override
            {
                Reflective::Transfer(transferer);

                TransferFieldInfo(sceneName);
            }
        };

        CustomReflectionInner(Settings)

        static ObjectPtr<EditorSystem> editorSystem;
        static ObjectPtr<SceneWindow> sceneWindow;
        static ObjectPtr<HierarchyWindow> hierarchyWindow;
        static ObjectPtr<InspectorWindow> inspectorWindow;
        static ObjectPtr<ProfilerWindow> profilerWindow;
        static ObjectPtr<ConsoleWindow> consoleWindow;
        static ObjectPtr<ProjectWindow> projectWindow;
        static ObjectPtr<GameWindow> gameWindow;
        static bool isPlaying;
        static std::string playScene;
        static ObjectPtrBase copying;

        static void NewScene();
        /**
         * \brief 使用ProjectWindow提供的保存功能是没有用的，必须要使用该函数才能正确保存
         * 因为Scene文件是特殊的，实际上本身是不被保存在内存中的
         * 所以在导出前它会先导入该文件，结果意外的覆盖了当前场景的改动
         * 如果Scene文件在导出前已经加载到内存中，则此时不会重新导入，但依旧存在问题
         * 当前场景不代表Scene文件，所以Scene文件中可能缺少后添加的一些物体，但本身存在的物体的修改此时是可以被保存的
         */
        static void SaveScene();
        CustomMenu("File/New Scene", NewScene)
        CustomMenu("File/Save Scene", SaveScene)
        static void Copy();
        static void Paste();
        static void Play();
        CustomMenu("Edit/Copy", Copy)
        CustomMenu("Edit/Paste", Paste)
        CustomMenuSeparator("Edit/");
        CustomMenu("Edit/Play | Stop", Play)

        void DrawMenu(const std::vector<std::string>& path, const std::function<void()>& func, size_t layer = 0);
        void OnDrawGUI() override;

        static void Initialize(const std::string& sceneName);
        static void OnEngineBegin();
        static void OnEngineEnd();
        CustomEngineBeginEvent(OnEngineBegin)
        CustomEngineEndEvent(OnEngineEnd)
    };

    CustomReflection(EditorSystem)

    void Run(const std::string& sceneName);
}
