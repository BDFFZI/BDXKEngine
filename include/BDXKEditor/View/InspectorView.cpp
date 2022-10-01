#include "InspectorView.h"

namespace BDXKEditor
{
    void Inspector::TransferInt(std::string key, int& value)
    {
        ImGui::InputInt(key.c_str(), &value, 0.1f);
    }
    void Inspector::TransferFloat(std::string key, float& value)
    {
        ImGui::InputFloat(key.c_str(), &value, 0.1f);
    }
    void Inspector::TransferBool(std::string key, bool& value)
    {
        ImGui::Checkbox(key.c_str(), &value);
    }
    void Inspector::TransferVector2(std::string key, Vector2& value)
    {
        ImGui::InputFloat2(key.c_str(), reinterpret_cast<float*>(&value));
    }
    void Inspector::TransferVector3(std::string key, Vector3& value)
    {
        ImGui::InputFloat3(key.c_str(), reinterpret_cast<float*>(&value));
    }
    void Inspector::TransferVector4(std::string key, Vector4& value)
    {
        ImGui::InputFloat4(key.c_str(), reinterpret_cast<float*>(&value));
    }
    void Inspector::TransferColor(std::string key, Color& value)
    {
        ImGui::ColorEdit4(key.c_str(), reinterpret_cast<float*>(&value));
    }
    void Inspector::TransferRect(std::string key, Rect& value)
    {
        ImGui::InputFloat2(key.c_str(), reinterpret_cast<float*>(&value));
        ImGui::InputFloat2(("##" + key + "2").c_str(), reinterpret_cast<float*>(&value) + 2);
    }
    void Inspector::TransferString(std::string key, std::string& value)
    {
        ImGui::InputText(key.c_str(), value.data(), value.size());
    }
    void Inspector::TransferObjectPtr(std::string key, ObjectPtrBase& value)
    {
        Object* object = value.ToObjectBase();
        if (dynamic_cast<Component*>(object) != nullptr)
        {
            ImGui::Separator();
            ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 30);
            ImGui::BeginGroup();
            static_cast<ISerializable*>(object)->Transfer(*this);
            ImGui::EndGroup();
        }
        else
            ImGui::LabelText(key.c_str(), std::to_string(value.GetInstanceID()).c_str());
    }
    void Inspector::TransferBytes(std::string key, void* source, int size)
    {
        ImGui::LabelText(key.c_str(), "二进制数据"); // NOLINT(clang-diagnostic-format-security)
    }

    void InspectorView::OnDrawWindow()
    {
        Object* targetObject = target.ToObjectBase();
        if (targetObject != nullptr)
            static_cast<ISerializable*>(targetObject)->Transfer(inspector); // NOLINT(clang-diagnostic-format-security)
    }
}
