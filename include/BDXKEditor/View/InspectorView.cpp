#include "InspectorView.h"

#include "SceneView.h"
#include "BDXKEditor/System/EditorSystem.h"
#include "ImGuizmo/ImGuizmo.h"

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
        Object* object = value.ToObjectBase();
        if (object == nullptr)
        {
            ImGui::Button(GetFieldID("nullptr").c_str());
            ImGui::SameLine();
            ImGui::Text(GetFieldName().c_str());
        }
        else
        {
            if (dynamic_cast<Component*>(object) != nullptr)
            {
                ImGui::Separator();

                const std::string serializationID = Object::ParseSerializationID(object);
                ImGui::LabelText(GetFieldID().c_str(), (serializationID + " " + std::to_string(value.GetInstanceID())).c_str());

                ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 30);
                ImGui::BeginGroup();
                static_cast<ISerializable*>(object)->Transfer(*this);
                ImGui::EndGroup();

                ImGui::Separator();
            }
            else
            {
                const std::string serializationID = Object::ParseSerializationID(object);

                if (ImGui::Button(GetFieldID(serializationID + " " + std::to_string(value.GetInstanceID())).c_str()))
                    EditorSystem::focusing = value;
                ImGui::SameLine();
                ImGui::Text(GetFieldName().c_str());
            }
        }
    }
    void Inspector::TransferValue(char* source, int size)
    {
        ImGui::LabelText(GetFieldID().c_str(), ("二进制数据[" + std::to_string(size) + "B]").c_str());
        // NOLINT(clang-diagnostic-format-security)
    }
    void Inspector::TransferValue(ISerializable& value)
    {
        value.Transfer(*this);
    }
    void Inspector::TransferValue(std::vector<ObjectPtrBase>& vector)
    {
        ImGui::Separator();

        ImGui::Text(GetFieldName().c_str());

        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 30);
        ImGui::BeginGroup();
        int count = static_cast<int>(vector.size());
        TransferField("Size", count);
        for (int i = 0; i < count; i++)
        {
            TransferField("Item" + std::to_string(i), vector[i]);
        }
        ImGui::EndGroup();

        ImGui::Separator();
    }

    void InspectorView::OnDrawWindow()
    {
        target = EditorSystem::focusing;
        Object* targetObject = target.ToObjectBase();
        if (targetObject != nullptr)
            static_cast<ISerializable*>(targetObject)->Transfer(inspector); // NOLINT(clang-diagnostic-format-security)
    }
}
