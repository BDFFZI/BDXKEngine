#include "InspectorWindow.h"
#include "BDXKEditor/Editor/Core/Editor.h"
#include "BDXKEngine/Platform/Serialization/Serialization.h"
#include "imgui/imgui.h"

namespace BDXKEditor
{
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
        if (isSpecial == true && ImGui::Button("Close InspectorWindow", {ImGui::GetContentRegionAvail().x, 0}))
        {
            DestroyImmediate(this);
            return;
        }

        if (target.IsNotNull())
        {
            if (isSpecial == false && ImGui::Button("Clone InspectorWindow", {ImGui::GetContentRegionAvail().x, 0}))
            {
                const ObjectPtr<InspectorWindow> window = Create<InspectorWindow>();
                window->SetName(target->GetName() + "##" + std::to_string(target.GetInstanceID()));
                window->target = target;
                window->isSpecial = true;
                window->Show();
            }

            Editor* editor = Editor::GetEditor(*target.ToObjectBase());
            editor->SetGui(&gui);
            editor->SetTarget(target);
            editor->DrawInspectorGUI();
        }
    }
}
