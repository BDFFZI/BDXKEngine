#include "EditorWindow.h"
#include "imgui/imgui.h"


namespace BDXKEditor
{
    std::vector<ObjectPtr<EditorWindow>> EditorWindow::windows = {};

    bool EditorWindow::GetCloseable() const
    {
        return closeable;
    }
    void EditorWindow::SetCloseable(bool closeable)
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

    void EditorWindow::OnDrawGUI()
    {
        bool isOpen = true;
        ImGui::Begin(GetName().c_str(), closeable ? &isOpen : nullptr, HasMenu() ? ImGuiWindowFlags_MenuBar : 0);
        if (isOpen == false && closeable) //用户尝试关闭窗口
        {
            DestroyImmediate(this);
            ImGui::End();
            return;
        }

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
