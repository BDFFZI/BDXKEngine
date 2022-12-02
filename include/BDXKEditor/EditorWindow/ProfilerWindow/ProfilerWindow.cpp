﻿// ReSharper disable CppClangTidyClangDiagnosticFormatSecurity
#include "ProfilerWindow.h"
#include "BDXKEngine/Platform/Resources/Resources.h"
#include "imgui/imgui.h"

namespace BDXKEditor
{
    char typeSelect[128];
    void ProfilerWindow::SetClickObjectEvent(const std::function<void(const ObjectPtrBase&)>& clickObjectEvent)
    {
        this->clickObjectEvent = clickObjectEvent;
    }

    void ProfilerWindow::OnGUI()
    {
        if (ImGui::Button("Clear"))
        {
            typeSelect[0] = '\0';
        }

        ImGui::SameLine();
        ImGui::InputText("Type", typeSelect, sizeof(typeSelect));
        const Reflection* reflection = Reflection::FindReflection(typeSelect);

        if (ImGui::BeginTable("Title", 6))
        {
            ImGui::TableNextColumn();
            ImGui::TableHeader("InstanceID");
            ImGui::TableNextColumn();
            ImGui::TableHeader("Name");
            ImGui::TableNextColumn();
            ImGui::TableHeader("Type");
            ImGui::TableNextColumn();
            ImGui::TableHeader("IsResource");
            ImGui::TableNextColumn();
            ImGui::TableHeader("IsActivating");
            ImGui::TableNextColumn();
            ImGui::TableHeader("IsEnabling");

            ImGui::EndTable();
        }

        ImGui::BeginChild("##");

        const auto Objects = GetObjects();
        if (ImGui::BeginTable("Content", 6))
        {
            for (const auto& [instanceID,object] : Objects)
            {
                if (object->IsInstantiated() == false)continue;
                if (reflection != nullptr && reflection->CanReceiveFrom(object) == false)continue;

#define GetUIID(name,type) ((name) + "##"##type + std::to_string(instanceID)).c_str()

                bool isResource = Resources::IsResource(object);
                const ScriptableObject* scriptableObject = dynamic_cast<ScriptableObject*>(object);

                ImGui::TableNextColumn();
                ImGui::Text(std::to_string(instanceID).c_str());
                ImGui::TableNextColumn();
                if (ImGui::Button(GetUIID(object->GetName(), "Name")))
                {
                    if (clickObjectEvent != nullptr)clickObjectEvent(object);
                }
                ImGui::TableNextColumn();
                if (ImGui::Button(GetUIID(object->GetType(), "Type")))
                {
                    std::string type = object->GetType();
                    strcpy_s(typeSelect, type.data());
                }
                ImGui::TableNextColumn();
                ImGui::Checkbox("##", &isResource);

                if (scriptableObject == nullptr)
                {
                    ImGui::TableNextColumn();
                    ImGui::TableNextColumn();
                }
                else
                {
                    bool isActivating = scriptableObject->GetIsActivating();
                    bool isEnabling = scriptableObject->GetIsEnabling();

                    ImGui::TableNextColumn();
                    ImGui::Checkbox("##", &isActivating);
                    ImGui::TableNextColumn();
                    ImGui::Checkbox("##", &isEnabling);
                }

                ImGui::TableNextRow();
            }

            ImGui::EndTable();
        }

        ImGui::EndChild();
    }
}
