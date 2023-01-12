// ReSharper disable CppClangTidyClangDiagnosticFormatSecurity
#include "ProfilerWindow.h"
#include "BDXKEngine/Platform/Serialization/Serialization.h"
#include "imgui/imgui.h"

namespace BDXKEditor
{
    char typeSelect[128];
    void ProfilerWindow::SetClickObjectEvent(const std::function<void(const ObjectPtrBase&)>& clickObjectEvent)
    {
        this->clickObjectEvent = clickObjectEvent;
    }
    void ProfilerWindow::DrawObjects() const
    {
        if (ImGui::Button("Clear"))
        {
            typeSelect[0] = '\0';
        }

        ImGui::SameLine();
        ImGui::InputText("Type", typeSelect, sizeof(typeSelect));
        const Reflection* reflection = nullptr;
        if (Reflection::HasReflection(typeSelect))
            reflection = &Reflection::GetReflection(typeSelect);

        if (ImGui::BeginTable("Title", 6))
        {
            ImGui::TableNextColumn();
            ImGui::TableHeader("Type");
            ImGui::TableNextColumn();
            ImGui::TableHeader("InstanceID");
            ImGui::TableNextColumn();
            ImGui::TableHeader("IsResource");
            ImGui::TableNextColumn();
            ImGui::TableHeader("IsHandleable");
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

                bool isResource = Serialization::IsPersistent(object);
                const ScriptableObject* scriptableObject = dynamic_cast<ScriptableObject*>(object);

                ImGui::TableNextColumn();
                if (ImGui::Button(GetUIID(object->GetType().substr(6), "Type")))
                {
                    std::string type = object->GetType();
                    strcpy_s(typeSelect, type.data());
                }
                ImGui::TableNextColumn();
                if (ImGui::Button(GetUIID(std::to_string(instanceID), "InstanceID")))
                {
                    if (clickObjectEvent != nullptr)clickObjectEvent(object);
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
                    bool isHandleable = scriptableObject->IsNotResource();
                    bool isActivating = scriptableObject->GetIsActivating();
                    bool isEnabling = scriptableObject->GetIsEnabling();

                    ImGui::TableNextColumn();
                    ImGui::Checkbox("##", &isHandleable);
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
    void ProfilerWindow::DrawGuids() const
    {
        if (ImGui::BeginTable("Title", 3))
        {
            ImGui::TableNextColumn();
            ImGui::TableHeader("Guid");
            ImGui::TableNextColumn();
            ImGui::TableHeader("InstanceID");
            ImGui::TableNextColumn();
            ImGui::TableHeader("IsMainGuid");

            ImGui::EndTable();
        }

        if (ImGui::BeginTable("Content", 3))
        {
            for (auto& [guid,instanceID] : ObjectGuid::GetAllGuids())
            {
                if (FindObjectOfInstanceID(instanceID) == nullptr)continue;

                ImGui::TableNextColumn();
                ImGui::Text(guid.c_str());

                ImGui::TableNextColumn();
                if (ImGui::Button(std::to_string(instanceID).c_str()))
                    if (clickObjectEvent != nullptr)
                        clickObjectEvent(FindObjectOfInstanceID(instanceID));

                ImGui::TableNextColumn();
                bool isMain = ObjectGuid::IsMainGuid(guid);
                ImGui::Checkbox("##", &isMain);

                ImGui::TableNextRow();
            }

            ImGui::EndTable();
        }
    }

    void ProfilerWindow::OnGUI()
    {
        if (ImGui::BeginTabBar("Target"))
        {
            if (ImGui::BeginTabItem("Objects"))
            {
                DrawObjects();
                ImGui::EndTabItem();
            }
            if (ImGui::BeginTabItem("Guids"))
            {
                DrawGuids();
                ImGui::EndTabItem();
            }
            ImGui::EndTabBar();
        }
    }
}
