#include "ProjectWindow.h"
#include <filesystem>
#include "BDXKEditor/Function/Assets.h"
#include "BDXKEditor/Function/ProjectSettings.h"
#include "BDXKEngine/Platform/GUI/GUI.h"
#include "BDXKEngine/Base/Package/Map.h"
#include "imgui/imgui.h"

namespace BDXKEditor
{
    ObjectPtrBase selectedItemObject;
    bool isPopupShowing = false; //表示右键菜单是否显示中，锁定每帧只显示一次，不然在TreeNode中会有问题（可能imgui用法错了）

    void ProjectWindow::SetClickObjectEvent(const std::function<void(const ObjectPtrBase&)>& clickObjectEvent)
    {
        this->clickObjectEvent = clickObjectEvent;
    }

    bool ShowFileMenu(const std::string& assetPath)
    {
        if (ImGui::Button("ReImport"))
        {
            Assets::Load(assetPath, true);
            return true;
        }
        if (ImGui::Button("Save & ReImport"))
        {
            ObjectPtrBase object = Assets::Load(assetPath);
            Assets::Save(assetPath, object);
            Assets::Load(assetPath, true);
            return true;
        }

        return false;
    }
    void ProjectWindow::ShowDirectory(const std::filesystem::path& directoryEntry)
    {
        //统计目录信息
        const std::string directoryPath = directoryEntry.string();
        const std::string directoryName = ParseFileName(directoryPath);

        std::filesystem::directory_iterator iterator = std::filesystem::directory_iterator{directoryPath};
        const bool unfold = ImGui::TreeNodeEx(
            directoryName.c_str(),
            (isUnfolding[directoryPath] ? ImGuiTreeNodeFlags_DefaultOpen : 0)
            | (iterator == end(iterator) ? ImGuiTreeNodeFlags_Leaf : 0)
        );
        isUnfolding[directoryPath] = unfold;

        if (unfold)
        {
            //统计目录下元素
            std::vector<std::filesystem::path> directories = {};
            std::vector<std::filesystem::path> files = {};
            for (const auto& child : std::filesystem::directory_iterator{directoryPath})
                if (child.is_directory())directories.emplace_back(child.path());
                else files.emplace_back(child.path());

            //显示目录
            for (auto& path : directories)
                ShowDirectory(path);

            //显示文件
            for (auto& path : files)
            {
                if (path.extension() != ".importer")
                {
                    const std::string filePath = path.string();
                    const std::string assetPath = filePath.substr(Assets::GetRootPath().size() + 1);

                    if (Assets::IsCanImport(assetPath) == false)
                    {
                        ImGui::Text(ParseFileName(path.string()).c_str());
                    }
                    else
                    {
                        ImGui::PushID(assetPath.c_str());
                        if (ImGui::Button(ParseFileName(path.string()).c_str())) //查看资源属性
                        {
                            selectedItemObject = Assets::Load(assetPath);
                            if (clickObjectEvent != nullptr)clickObjectEvent(selectedItemObject);
                        }
                        if (GUI::IsDragSource(selectedItemObject)) //拖拽资源
                        {
                            selectedItemObject = Assets::Load(assetPath);
                        }
                        //右键菜单
                        if (isPopupShowing == false && Assets::IsCanImport(assetPath) && ImGui::BeginPopupContextItem("##FileMenu"))
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
        ShowDirectory(std::filesystem::path(Assets::GetRootPath()));
    }

    void ProjectWindow::StaticConstructor()
    {
        Reflection::SetReflection<Map<std::string, bool>>();
        auto* isUnfoldingPackage = ProjectSettings::Load<Map<std::string, bool>>("ProjectWindow.settings");
        if (isUnfoldingPackage != nullptr)
        {
            isUnfolding = isUnfoldingPackage->ToUnorderedMap();
            delete isUnfoldingPackage;
        }
    }
    void ProjectWindow::StaticDestructor()
    {
        Map isUnfoldingPackage = {isUnfolding};
        ProjectSettings::Save("ProjectWindow.settings", &isUnfoldingPackage);
    }
}
