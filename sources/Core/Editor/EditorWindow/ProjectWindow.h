#pragma once
#include <filesystem>
#include "Core/Editor/Core/Editor/EditorWindow.h"

namespace BDXKEditor
{
    class ProjectWindow;
    class ProjectSetting : public Object, public SerializeEvent, public DeserializeEvent
    {
        friend ProjectWindow;

        inline static std::unordered_map<std::filesystem::path, bool> isUnfolding = {};

        inline static std::vector<std::string> keys;
        inline static std::vector<bool> values;
        void OnBeforeSerialize() override;
        void OnAfterDeserialize() override;
        void Transfer(Transferer& transferer) override;
    };
    CustomReflection(ProjectSetting)

    class ProjectWindow : public EditorWindow
    {
    public:
        void SetClickObjectEvent(const std::function<void(const ObjectPtrBase&)>& clickObjectEvent);

    private:
        ObjectPtrBase selectedItemObject;
        bool isPopupShowing = false; //表示右键菜单是否显示中，锁定每帧只显示一次，不然在TreeNode中会有问题（可能imgui用法错了）
        std::function<void(const ObjectPtrBase&)> clickObjectEvent;

        void ShowDirectory(const std::string& name, const std::filesystem::path& directoryEntry);
        void OnGUI() override;

        static void OnEngineStart();
        CustomEngineStartEvent(OnEngineStart, 0)
    };
    CustomReflection(ProjectWindow)
}
