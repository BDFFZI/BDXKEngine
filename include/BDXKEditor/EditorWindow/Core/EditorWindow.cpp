#include "EditorWindow.h"
#include "imgui/imgui.h"


namespace BDXKEditor
{
    std::vector<ObjectPtr<EditorWindow>> EditorWindow::windows = {};

    void EditorWindow::Show()
    {
        SetIsEnabling(true);
    }

    void EditorWindow::OnGUI()
    {
    }
    void EditorWindow::OnDrawGUI()
    {
        ImGui::Begin(GetName().c_str());

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
