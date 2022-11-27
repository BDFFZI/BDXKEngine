#include "InspectorWindow.h"
#include "BDXKEditor/Editor/Core/Editor.h"

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
        if (target.IsNotNull())
        {
            Editor* editor = Editor::GetEditor(target->GetType());
            editor->SetGui(&gui);
            editor->SetTarget(target);
            editor->DrawInspectorGUI();
        }
    }
}
