#include "Editor.h"

namespace BDXKEditor
{
    void Editor::SetTarget(ObjectPtrBase& target)
    {
        this->target = target;
    }
    void Editor::SetGui(transferer& gui)
    {
        this->gui = &gui;
    }

    void Editor::OnInspectorGUI()
    {
        target.ToObjectBase()->Transfer(*gui);
    }
    void Editor::OnSceneGUI()
    {
    }

    void Editor::Transfer(transferer& transferer)
    {
        SwitchableObject::Transfer(transferer);

        transferer.TransferField("target", target);
    }
}
