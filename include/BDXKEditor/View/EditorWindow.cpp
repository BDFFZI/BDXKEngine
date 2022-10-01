#include "EditorWindow.h"

#include "imgui/imgui.h"


namespace BDXKEditor
{
    void EditorWindow::Show()
    {
        SetIsEnabling(true);
        OnShow();
    }

    void EditorWindow::OnDrawWindow()
    {
    }

    void EditorWindow::OnShow()
    {
    }

    void EditorWindow::OnDrawGUI()
    {
        ImGui::Begin(GetName().c_str());

        OnDrawWindow();

        ImGui::End();
    }
}
