// ReSharper disable CppClangTidyClangDiagnosticFormatSecurity
#include "ProfilerWindow.h"
#include "BDXKEngine/Platform/Resources/Resources.h"
#include "imgui/imgui.h"

namespace BDXKEditor
{
    char type[100];
    void ProfilerWindow::OnGUI()
    {
        ImGui::InputText("Type", type, sizeof(type));
        const Reflection* reflection = Reflection::FindReflection(type);

        const auto Objects = GetObjects();
        if (ImGui::BeginTable("Objects", 5))
        {
            ImGui::TableNextColumn();
            ImGui::TableHeader("InstanceID");
            ImGui::TableNextColumn();
            ImGui::TableHeader("Name");
            ImGui::TableNextColumn();
            ImGui::TableHeader("IsResource");
            ImGui::TableNextColumn();
            ImGui::TableHeader("IsActivating");
            ImGui::TableNextColumn();
            ImGui::TableHeader("IsEnabling");
            ImGui::TableNextRow();

            for (const auto& [instanceID,object] : Objects)
            {
                if (object->IsInstantiated() == false)continue;
                if (reflection != nullptr && reflection->CanReceiveFrom(object) == false)continue;

                bool isResource = Resources::IsResource(object);
                const ScriptableObject* scriptableObject = dynamic_cast<ScriptableObject*>(object);

                ImGui::TableNextColumn();
                ImGui::Text(std::to_string(instanceID).c_str());
                ImGui::TableNextColumn();
                ImGui::Button(object->GetName().c_str());
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
    }
}
