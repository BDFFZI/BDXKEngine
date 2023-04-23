#include "EditorWindow.h"
#include <imgui.h>

#include "Core/Editor/Core/Settings/Settings.h"


namespace BDXKEditor
{
    bool EditorWindow::GetCloseable() const
    {
        return closeable;
    }
    void EditorWindow::SetCloseable(const bool closeable)
    {
        this->closeable = closeable;
    }
    void EditorWindow::Show()
    {
        SetIsEnabling(true);
    }
    bool EditorWindow::HasMenu()
    {
        return false;
    }

    void EditorWindow::OnLateUpdate()
    {
        bool isOpen = true;
        ImGui::Begin(GetName().c_str(), closeable ? &isOpen : nullptr, HasMenu() ? ImGuiWindowFlags_MenuBar : 0);
        if (isOpen == false && closeable) //用户尝试关闭窗口
        {
            DestroyObject(this);
            ImGui::End();
            return;
        }

        OnGUI();

        ImGui::End();
    }

    void EditorWindow::OnEngineStart()
    {
        gameObject = GameObject::Create("EditorWindows", nullptr, Scene::GetDurableScene());
    }
}
