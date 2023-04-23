#pragma once
#include "Core/Runtime/CoreRuntime.h"
#include "Core/Editor/EditorWindow/ConsoleWindow.h"
#include "Core/Editor/EditorWindow/HierarchyWindow.h"
#include "Core/Editor/EditorWindow/InspectorWindow.h"
#include "Core/Editor/EditorWindow/ProfilerWindow.h"
#include "Core/Editor/EditorWindow/ProjectWindow.h"
#include "Core/Editor/Core/Editor/EditorMenu.h"
#include "Core/Runtime/Framework/Actor/Behavior.h"

namespace BDXKEditor
{
    using namespace BDXKEngine;

    class CoreEditor;

    class EditorSetting : public Object
    {
        friend CoreEditor;

        inline static std::string sceneName = {};
        void Transfer(Transferer& transferer) override;
    };
    CustomReflection(EditorSetting)

    class CoreEditor : public Behavior, public LateUpdateEvent
    {
    public:
        static void AddClickObjectEvent(const std::function<void(const ObjectPtrBase&)>& clickObjectEvent);
        static const ObjectPtr<HierarchyWindow>& GetHierarchyView();
        static const ObjectPtr<InspectorWindow>& GetInspectorView();

    private:
        inline static bool isPlaying;
        inline static ObjectPtrBase copying;
        inline static std::vector<std::function<void(const ObjectPtrBase&)>> clickObjectEvents;
        inline static ObjectPtr<CoreEditor> editorSystem;
        inline static ObjectPtr<HierarchyWindow> hierarchyWindow;
        inline static ObjectPtr<InspectorWindow> inspectorWindow;
        inline static ObjectPtr<ProfilerWindow> profilerWindow;
        inline static ObjectPtr<ConsoleWindow> consoleWindow;
        inline static ObjectPtr<ProjectWindow> projectWindow;

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
        
        void OnLateUpdate() override;

        static void Initialize(const std::string& sceneName);
        static void OnEngineStart();
        CustomEngineStartEvent(OnEngineStart, 5)
    };

    CustomReflection(CoreEditor)
}
