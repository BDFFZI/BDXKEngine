#include "EditorWindow.h"
#include "imgui/imgui.h"


namespace BDXKEditor
{
    void EditorWindow::Show()
    {
        SetIsEnabling(true);
    }
    
    void EditorWindow::OnGUI()
    {
    }
    void EditorWindow::OnDrawGUI()
    {
        ImGui::Begin(this->GetType().c_str());

        OnGUI();

        ImGui::End();
    }
}
