#include "Editor.h"

namespace BDXKEditor
{
    void Editor::SetTarget(ObjectPtrBase& target)
    {
        this->target = target;
    }
    void Editor::SetGui(Transferrer& gui)
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

    void Editor::Transfer(Transferrer& transferrer)
    {
        SwitchableObject::Transfer(transferrer);

        transferrer.TransferField("target", target);
    }
}
