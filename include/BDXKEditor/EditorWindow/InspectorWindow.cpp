#include "InspectorWindow.h"
#include "SceneWindow.h"
#include "BDXKEditor/EditorSystem.h"
#include "BDXKEditor/Editor/Editor.h"

namespace BDXKEditor
{
    void Inspector::TransferValue(int& value)
    {
        ImGui::InputInt(GetFieldID().c_str(), &value, 1);
    }
    void Inspector::TransferValue(float& value)
    {
        ImGui::DragFloat(GetFieldID().c_str(), &value, 0.05f);
    }
    void Inspector::TransferValue(bool& value)
    {
        ImGui::Checkbox(GetFieldID().c_str(), &value);
    }
    void Inspector::TransferValue(Vector2& value)
    {
        ImGui::DragFloat2(GetFieldID().c_str(), reinterpret_cast<float*>(&value), 0.05f);
    }
    void Inspector::TransferValue(Vector3& value)
    {
        ImGui::DragFloat3(GetFieldID().c_str(), reinterpret_cast<float*>(&value), 0.05f);
    }
    void Inspector::TransferValue(Vector4& value)
    {
        ImGui::DragFloat4(GetFieldID().c_str(), reinterpret_cast<float*>(&value), 0.05f);
    }
    void Inspector::TransferValue(Color& value)
    {
        ImGui::ColorEdit4(GetFieldID().c_str(), reinterpret_cast<float*>(&value));
    }
    void Inspector::TransferValue(Rect& value)
    {
        ImGui::InputFloat2((GetFieldPath() + "Site").c_str(), reinterpret_cast<float*>(&value));
        ImGui::InputFloat2((GetFieldPath() + "Size").c_str(), reinterpret_cast<float*>(&value) + 2);
    }
    void Inspector::TransferValue(std::string& value)
    {
        ImGui::InputText(GetFieldID().c_str(), value.data(), value.size() + 1);
    }
    void Inspector::TransferValue(ObjectPtrBase& value)
    {
        const BDXKObject* object = value.ToObjectBase();
        if (object == nullptr)
        {
            ImGui::Button(GetFieldID("nullptr").c_str());
            ImGui::SameLine();
            ImGui::Text(GetFieldName().c_str());
        }
        else
        {
            const std::string typeID = object->GetTypeID();
            if (ImGui::Button(GetFieldID(typeID + " " + std::to_string(value.GetInstanceID())).c_str()))
                EditorSystem::SetFocusing(value);
            ImGui::SameLine();
            ImGui::Text(GetFieldName().c_str());
        }
    }
    void Inspector::TransferValue(char* source, int size)
    {
        ImGui::LabelText(GetFieldID().c_str(), ("二进制数据[" + std::to_string(size) + "B]").c_str());
        // NOLINT(clang-diagnostic-format-security)
    }
    void Inspector::TransferValue(Serialization& value)
    {
        value.Transfer(*this);
    }

    void InspectorWindow::OnGUI()
    {
        if (lastTarget != EditorSystem::GetFocusing())
        {
            lastTarget = EditorSystem::GetFocusing();
            if (lastTarget.IsNotNull())
            {
                editor = EditorSystem::GetEditor(lastTarget.ToObjectBase()->GetTypeID());
                editor->SetTarget(lastTarget);
                editor->SetGui(inspector);
                editor->SetIsEnabling(true);
            }
            else
                editor = nullptr;
        }

        if (editor.IsNotNull())editor->OnInspectorGUI();
    }
}
