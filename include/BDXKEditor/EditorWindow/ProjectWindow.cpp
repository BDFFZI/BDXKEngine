#include "ProjectWindow.h"
#include <filesystem>
#include "BDXKEditor/Function/Assets.h"
#include "BDXKEngine/Platform/GUI/GUI.h"
#include "imgui/imgui.h"

namespace BDXKEditor
{
    ObjectPtrBase selectedItemObject;

    void ProjectWindow::SetClickObjectEvent(const std::function<void(const ObjectPtrBase&)>& clickObjectEvent)
    {
        this->clickObjectEvent = clickObjectEvent;
    }

    void ProjectWindow::ShowDirectory(const std::filesystem::directory_entry& directoryEntry)
    {
        const std::string path = directoryEntry.path().string();
        const std::string name = ParseFileName(path);
        if (ImGui::TreeNode(name.c_str()))
        {
            const std::filesystem::directory_iterator iterator = std::filesystem::directory_iterator{path};
            for (const auto& item : iterator)
            {
                if (item.is_directory())
                    ShowDirectory(item);
                else
                {
                    if (ImGui::Button(ParseFileName(item.path().string()).c_str()))
                    {
                        const std::string itemPath = item.path().string().substr(Assets::GetRootPath().size() + 1);
                        selectedItemObject = Assets::Load(itemPath);
                        if (clickObjectEvent != nullptr)clickObjectEvent(selectedItemObject);
                    }
                    if (GUI::BeginDragDrop(selectedItemObject))
                    {
                        const std::string itemPath = item.path().string().substr(Assets::GetRootPath().size() + 1);
                        selectedItemObject = Assets::Load(itemPath);
                    }
                }
            }

            ImGui::TreePop();
        }
    }

    void ProjectWindow::OnGUI()
    {
        ShowDirectory(std::filesystem::directory_entry(Assets::GetRootPath()));
    }
}
