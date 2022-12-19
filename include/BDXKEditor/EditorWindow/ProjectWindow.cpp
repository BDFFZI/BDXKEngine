#include "ProjectWindow.h"
#include <filesystem>
#include "BDXKEditor/Function/Assets.h"
#include "BDXKEngine/Platform/GUI/GUI.h"
#include "imgui/imgui.h"

namespace BDXKEditor
{
    ObjectPtrBase selectedItemObject;
    bool contextMenu = false;

    bool DirectoryMenu(const std::string& path)
    {
        // if (ImGui::Button("Create Folder"))
        // {
        //     std::filesystem::create_directory(path + "/Unnamed");
        //     return true;
        // }
        // if (ImGui::Button("Delete Folder"))
        // {
        //     std::filesystem::remove(path);
        //     return true;
        // }
        // if (ImGui::Button("Rename Folder"))
        // {
        //     return true;
        // }

        return false;
    }

    bool FileMenu(const std::string& path)
    {
        const std::string fileName = ParseFileName(path);
        if (ImGui::Button("ReImport"))
        {
            Assets::Load(path.substr(Assets::GetRootPath().size() + 1), true);
            return true;
        }

        return false;
    }

    void ProjectWindow::SetClickObjectEvent(const std::function<void(const ObjectPtrBase&)>& clickObjectEvent)
    {
        this->clickObjectEvent = clickObjectEvent;
    }

    void ProjectWindow::ShowDirectory(const std::filesystem::directory_entry& directoryEntry)
    {
        //统计目录信息
        const std::string directoryPath = directoryEntry.path().string();
        const std::string directoryName = ParseFileName(directoryPath);

        std::filesystem::directory_iterator iterator = std::filesystem::directory_iterator{directoryPath};
        const bool unfold = ImGui::TreeNodeEx(
            directoryName.c_str(),
            ImGuiTreeNodeFlags_DefaultOpen |
            (iterator == end(iterator) ? ImGuiTreeNodeFlags_Leaf : 0)
        );
        //路径菜单
        if (contextMenu == false && ImGui::BeginPopupContextItem("##DirectoryMenu"))
        {
            contextMenu = true;
            if (DirectoryMenu(directoryPath))
            {
                ImGui::CloseCurrentPopup();
                ImGui::EndPopup();
                ImGui::TreePop();
                return;
            }
            ImGui::EndPopup();
        }
        if (unfold)
        {
            for (const auto& child : std::filesystem::directory_iterator{directoryPath})
            {
                if (child.is_directory())
                    ShowDirectory(child);
                else if (child.path().extension() != ".importer")
                {
                    std::string filePath = child.path().string();
                    if (ImGui::Button(ParseFileName(child.path().string()).c_str()))
                    {
                        const std::string itemPath = filePath.substr(Assets::GetRootPath().size() + 1);
                        selectedItemObject = Assets::Load(itemPath);
                        if (clickObjectEvent != nullptr)clickObjectEvent(selectedItemObject);
                    }
                    if (GUI::BeginDragDrop(selectedItemObject))
                    {
                        const std::string itemPath = filePath.substr(Assets::GetRootPath().size() + 1);
                        selectedItemObject = Assets::Load(itemPath);
                    }
                    //文件菜单
                    if (contextMenu == false && ImGui::BeginPopupContextItem("##FileMenu"))
                    {
                        contextMenu = true;
                        if (FileMenu(filePath))
                            ImGui::CloseCurrentPopup();
                        ImGui::EndPopup();
                    }
                }
            }

            ImGui::TreePop();
        }
    }

    void ProjectWindow::OnGUI()
    {
        contextMenu = false;
        ShowDirectory(std::filesystem::directory_entry(Assets::GetRootPath()));

        // if (ImGui::IsMouseClicked(ImGuiMouseButton_Right))
        // {
        //     leftMenu = 10;
        //     ImGui::SetNextWindowPos(Vector2{ImGui::GetMousePos()} - Vector2{100, 100});
        //     ImGui::SetNextWindowSize({1000, 1000});
        // }
        //
        // if (leftMenu != 0)
        // {
        //     ImGui::Begin("##CreateAssetMenu", nullptr, ImGuiWindowFlags_NoTitleBar);
        //     ImGui::End();
        //
        //     if (leftMenu > 1)
        //         leftMenu--;
        //     else if (ImGui::IsWindowHovered() == false)
        //         leftMenu = 0;
        // }
    }
}
