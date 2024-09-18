#include "EditorMenu.h"
#include "Core/Runtime/Base/Data/String/String.h"
#include "Core/Runtime/Function/GUI/GUI.h"

namespace BDXKEditor
{
    std::vector<std::tuple<std::string, std::function<void()>>>& EditorMenu::GetMenuItems()
    {
        return menuItems;
    }
    void EditorMenu::AddMenuItem(const std::string& name, const std::function<void()>& createFunc)
    {
        menuItems.emplace_back(name, createFunc);
    }
    void EditorMenu::SetMenuBarColor(const Color color)
    {
        barColor = color;
    }

    void EditorMenu::DrawMenu(const std::vector<std::string>& path, const std::function<void()>& func, const size_t layer)
    {
        if (layer > path.size())
            return;

        if (layer == path.size() - 1)
        {
            if (ImGui::MenuItem(path.back().c_str()))
            {
                func();
            }
        }
        else
        {
            if (ImGui::BeginMenu(path[layer].c_str()))
            {
                DrawMenu(path, func, layer + 1);
                ImGui::EndMenu();
            }
        }
    }
    void EditorMenu::OnEngineStart()
    {
        barColor = Color{ImGui::GetStyleColorVec4(ImGuiCol_MenuBarBg)};
    }
    void EditorMenu::OnEngineUpdate()
    {
        //绘制菜单栏
        ImGui::PushStyleColor(ImGuiCol_MenuBarBg, barColor);
        if (ImGui::BeginMainMenuBar())
        {
            //菜单项
            std::vector<std::string> path = {};
            for (auto& [name,func] : GetMenuItems())
            {
                String::Split(name, "/", path);
                DrawMenu(path, func);
            }

            ImGui::EndMainMenuBar();
        }
        ImGui::PopStyleColor();
    }
}
