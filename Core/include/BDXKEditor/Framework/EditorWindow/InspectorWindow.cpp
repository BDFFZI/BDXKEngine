#include "InspectorWindow.h"
#include "BDXKEditor/Base/Editor/Core/Editor.h"
#include "imgui/imgui.h"

namespace BDXKEditor
{
    const ObjectPtrBase& InspectorWindow::GetTarget()
    {
        return target;
    }
    void InspectorWindow::SetTarget(const ObjectPtrBase& target)
    {
        this->target = target;
    }
    void InspectorWindow::OnAwake()
    {
        gui.SetClickObjectEvent(std::function([this](const ObjectPtrBase& target) { SetTarget(target); }));
    }
    void InspectorWindow::OnGUI()
    {
        if (target.IsNotNull())
        {
            if (ImGui::Button("Clone InspectorWindow", {ImGui::GetContentRegionAvail().x, 0}))
            {
                const ObjectPtr<InspectorWindow> window = Create<InspectorWindow>();
                window->target = target;
                window->Show();
            }

            Editor::GetEditor(target, gui).DrawInspectorGUI();
        }
    }
}
