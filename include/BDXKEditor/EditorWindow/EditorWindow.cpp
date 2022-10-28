#include "imgui/imgui.h"
#include "EditorWindow.h"


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
        ImGui::Begin(this->GetTypeID().c_str());

        OnGUI();

        ImGui::End();
    }
}
