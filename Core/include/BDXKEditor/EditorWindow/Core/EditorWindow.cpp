#include "EditorWindow.h"
#include "imgui/imgui.h"


namespace BDXKEditor
{
    std::vector<ObjectPtr<EditorWindow>> EditorWindow::windows = {};

    void EditorWindow::Show()
    {
        SetIsEnabling(true);
    }
    bool EditorWindow::HasMenu()
    {
        return false;
    }

    void EditorWindow::OnDrawGUI()
    {
        ImGui::Begin(
            GetName().c_str(),
            nullptr,
            HasMenu() ? ImGuiWindowFlags_MenuBar : 0
        );

        OnGUI();

        ImGui::End();
    }
    void EditorWindow::Awake()
    {
        ScriptableObject::Awake();

        windows.emplace_back(this);
    }
    void EditorWindow::Destroy()
    {
        std::erase(windows, this);

        ScriptableObject::Destroy();
    }
}
