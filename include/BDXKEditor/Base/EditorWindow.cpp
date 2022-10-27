#include "BDXKEditor/Base/EditorWindow.h"

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
        ImGui::Begin(this->GetTypeID().c_str());

        OnGUI();

        ImGui::End();
    }
}
