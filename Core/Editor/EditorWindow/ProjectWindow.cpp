#include "ProjectWindow.h"
#include <filesystem>
#include "Core/Editor/Core/Assets/AssetImporter.h"
#include "Core/Editor/Core/Assets/AssetDatabase.h"
#include "Core/Editor/Core/Settings/Settings.h"
#include "Core/Runtime/Function/GUI/GUI.h"

namespace BDXKEditor
{
    void ProjectSetting::OnBeforeSerialize()
    {
        keys.clear();
        values.clear();
        for (auto [key,value] : isUnfolding)
        {
            keys.push_back(key.string());
            values.push_back(value);
        }
    }
    void ProjectSetting::OnAfterDeserialize()
    {
        isUnfolding.clear();
        const size_t count = keys.size();
        for (size_t i = 0; i < count; i++)
            isUnfolding[keys[i]] = values[i];
    }
    void ProjectSetting::Transfer(Transferer& transferer)
    {
        Object::Transfer(transferer);
        TransferFieldInfo(keys);
        TransferFieldInfo(values);
    }

    void ProjectWindow::SetClickObjectEvent(const std::function<void(const ObjectPtrBase&)>& clickObjectEvent)
    {
        this->clickObjectEvent = clickObjectEvent;
    }

    bool ShowFileMenu(const std::string& assetPath)
    {
        if (ImGui::Button("ReLoad"))
        {
            AssetDatabase::ReLoad(assetPath);
            return true;
        }
        if (ImGui::Button("Save & ReLoad"))
        {
            const ObjectPtrBase object = AssetDatabase::Load(assetPath);
            AssetDatabase::Save(assetPath, object);
            AssetDatabase::ReLoad(assetPath);
            return true;
        }

        return false;
    }
    void ProjectWindow::ShowDirectory(const std::string& name, const std::filesystem::path& directoryEntry)
    {
        //统计目录信息
        std::filesystem::directory_iterator iterator = std::filesystem::directory_iterator{directoryEntry};
        const bool unfold = ImGui::TreeNodeEx(
            name.c_str(),
            (ProjectSetting::isUnfolding[directoryEntry] ? ImGuiTreeNodeFlags_DefaultOpen : 0)
            | (iterator == end(iterator) ? ImGuiTreeNodeFlags_Leaf : 0)
        );
        ProjectSetting::isUnfolding[directoryEntry] = unfold;

        if (unfold)
        {
            //统计目录下元素
            std::vector<std::filesystem::path> directories = {};
            std::vector<std::filesystem::path> files = {};
            for (const auto& child : std::filesystem::directory_iterator{directoryEntry})
                if (child.is_directory())directories.emplace_back(child.path());
                else files.emplace_back(child.path());

            //显示目录
            for (auto& path : directories)
                ShowDirectory(path.filename().string(), path);

            //显示文件
            for (auto& path : files)
            {
                if (path.extension() != ".importer")
                {
                    const std::string assetPath = path.string();

                    if (AssetDatabase::IsCanImport(assetPath) == false)
                    {
                        ImGui::Text(path.filename().string().c_str());
                    }
                    else
                    {
                        ImGui::PushID(assetPath.c_str());
                        if (ImGui::Button(path.filename().string().c_str())) //查看资源属性
                        {
                            selectedItemObject = AssetDatabase::Load(assetPath);
                            if (clickObjectEvent != nullptr)
                                clickObjectEvent(selectedItemObject);
                        }
                        if (GUI::IsDragSource(selectedItemObject)) //拖拽资源
                        {
                            selectedItemObject = AssetDatabase::Load(assetPath);
                        }
                        //右键菜单
                        if (isPopupShowing == false && AssetDatabase::IsCanImport(assetPath) && ImGui::BeginPopupContextItem("##FileMenu"))
                        {
                            isPopupShowing = true;
                            if (ShowFileMenu(assetPath))
                                ImGui::CloseCurrentPopup();
                            ImGui::EndPopup();
                        }
                        ImGui::PopID();
                    }
                }
            }

            ImGui::TreePop();
        }
    }
    void ProjectWindow::OnGUI()
    {
        isPopupShowing = false;
        for (const auto& [name,path] : AssetDatabase::GetAllRefreshDirs())
            ShowDirectory(name, path);
    }
    void ProjectWindow::OnEngineStart()
    {
        Settings::SetEditorSetting<ProjectSetting>();
    }
}
