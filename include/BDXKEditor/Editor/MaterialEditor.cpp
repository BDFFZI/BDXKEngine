#include "MaterialEditor.h"

namespace BDXKEditor
{
    void MaterialEditor::OnInspectorGUI()
    {
        Editor::OnInspectorGUI();
        auto* material = GetTarget().ToObject<Material>();
        for (int i = 0; i < material->GetPassCount(); i++)
        {
            material->SetPassBlend(i, material->GetPassBlend(i));
            material->SetPassZTest(i, material->GetPassZTest(i));
            material->SetPassCull(i, material->GetPassCull(i));
        }
    }
}
