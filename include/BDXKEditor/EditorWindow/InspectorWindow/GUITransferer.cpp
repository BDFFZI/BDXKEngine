// ReSharper disable CppClangTidyClangDiagnosticFormatSecurity
#include "GUITransferer.h"
#include "BDXKEngine/Base/Object/Core/Object.h"
#include "imgui/imgui.h"

namespace BDXKEditor
{
    void GUITransferer::SetClickObjectEvent(const std::function<void(const ObjectPtrBase&)>& clickObjectEvent)
    {
        this->clickObjectEvent = clickObjectEvent;
    }
    void GUITransferer::PushPath(const std::string& key)
    {
        if (isInPath && fieldName.c_str()[0] != '#')
        {
            ImGui::Text(fieldName.c_str());
            isInPath = false;
        }

        fieldPath.append(key);
        fieldName = key;
        isInPath = true;
    }
    void GUITransferer::PopPath(std::string& key)
    {
        fieldPath.resize(fieldPath.size() - key.size());
        isInPath = false;
    }
    void GUITransferer::TransferInt(int& value) const
    {
        ImGui::InputInt(GetFieldID().c_str(), &value, 1);
    }
    void GUITransferer::TransferFloat(float& value) const
    {
        ImGui::DragFloat(GetFieldID().c_str(), &value, 0.05f);
    }
    void GUITransferer::TransferBool(bool& value) const
    {
        ImGui::Checkbox(GetFieldID().c_str(), &value);
    }
    void GUITransferer::TransferVector2(Vector2& value) const
    {
        ImGui::DragFloat2(GetFieldID().c_str(), reinterpret_cast<float*>(&value), 0.05f);
    }
    void GUITransferer::TransferVector3(Vector3& value) const
    {
        ImGui::DragFloat3(GetFieldID().c_str(), reinterpret_cast<float*>(&value), 0.05f);
    }
    void GUITransferer::TransferVector4(Vector4& value) const
    {
        ImGui::DragFloat4(GetFieldID().c_str(), reinterpret_cast<float*>(&value), 0.05f);
    }
    void GUITransferer::TransferMatrix4x4(Matrix4x4& value) const
    {
        Vector4 row0 = value.GetRow(0);
        Vector4 row1 = value.GetRow(1);
        Vector4 row2 = value.GetRow(2);
        Vector4 row3 = value.GetRow(3);
        ImGui::DragFloat4((GetFieldID() + "1").c_str(), reinterpret_cast<float*>(&row0), 0.05f);
        ImGui::DragFloat4((GetFieldID() + "2").c_str(), reinterpret_cast<float*>(&row1), 0.05f);
        ImGui::DragFloat4((GetFieldID() + "3").c_str(), reinterpret_cast<float*>(&row2), 0.05f);
        ImGui::DragFloat4((GetFieldID() + "4").c_str(), reinterpret_cast<float*>(&row3), 0.05f);
        value.SetRow(0, row0);
        value.SetRow(1, row1);
        value.SetRow(2, row2);
        value.SetRow(3, row3);
    }
    void GUITransferer::TransferColor(Color& value) const
    {
        ImGui::ColorEdit4(GetFieldID().c_str(), reinterpret_cast<float*>(&value));
    }
    void GUITransferer::TransferRect(Rect& value) const
    {
        ImGui::InputFloat2((GetFieldPath() + "Site").c_str(), reinterpret_cast<float*>(&value));
        ImGui::InputFloat2((GetFieldPath() + "Size").c_str(), reinterpret_cast<float*>(&value) + 2);
    }
    void GUITransferer::TransferString(std::string& value) const
    {
        ImGui::InputText(GetFieldID().c_str(), value.data(), value.size() + 1);
    }
    void GUITransferer::TransferObjectPtrBase(const ObjectPtrBase& value) const
    {
        const Object* object = value.ToObjectBase();
        if (object == nullptr)
        {
            ImGui::Button(GetFieldID("nullptr").c_str());
            ImGui::SameLine();
            ImGui::Text(GetFieldName().c_str());
        }
        else
        {
            const std::string typeID = object->GetType();
            if (ImGui::Button(GetFieldID(typeID + " " + std::to_string(value.GetInstanceID())).c_str()))
                if (clickObjectEvent != nullptr) clickObjectEvent(value);
            ImGui::SameLine();
            ImGui::Text(GetFieldName().c_str());
        }
    }
    void GUITransferer::TransferBytes(const std::vector<char>& value) const
    {
        ImGui::LabelText(GetFieldID().c_str(), ("二进制数据[" + std::to_string(value.size()) + "B]").c_str());
    }
}
