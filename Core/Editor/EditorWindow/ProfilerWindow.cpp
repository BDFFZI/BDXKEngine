// ReSharper disable CppClangTidyClangDiagnosticFormatSecurity
#include "ProfilerWindow.h"
#include "Core/Runtime/Base/Object/Serialization/ObjectSerialization.h"
#include <imgui.h>

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

        if (ImGui::BeginTable("Title", 5))
        {
            ImGui::TableNextColumn();
            ImGui::TableHeader("Name");
            ImGui::TableNextColumn();
            ImGui::TableHeader("ObjectID");
            ImGui::TableNextColumn();
            ImGui::TableHeader("IsResource");
            ImGui::TableNextColumn();
            ImGui::TableHeader("IsActivating");
            ImGui::TableNextColumn();
            ImGui::TableHeader("IsEnabling");

            ImGui::EndTable();
        }

        ImGui::BeginChild("##");

        const auto Objects = GetAllObjects();
        if (ImGui::BeginTable("Content", 5))
        {
            for (const auto& [objectID,object] : Objects)
            {
                if (object->GetIsAwaked() == false)continue;
                if (reflection != nullptr && reflection->CanReceiveFrom(object) == false)continue;

#define GetUIID(name,type) ((name) + "##"##type + std::to_string(objectID)).c_str()

                bool isResource = ObjectSerialization::IsPersistent(object);
                const SwitchableObject* scriptableObject = dynamic_cast<SwitchableObject*>(object);

                ImGui::TableNextColumn();
                if (ImGui::Button(GetUIID(object->GetName(), "Name")))
                {
                    std::string type = object->GetType();
                    strcpy_s(typeSelect, type.data());
                }
                ImGui::TableNextColumn();
                if (ImGui::Button(GetUIID(std::to_string(objectID), "ObjectID")))
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
    void ProfilerWindow::DrawGuids() const
    {
        if (ImGui::BeginTable("Title", 3))
        {
            ImGui::TableNextColumn();
            ImGui::TableHeader("Guid");
            ImGui::TableNextColumn();
            ImGui::TableHeader("ObjectID");
            ImGui::TableNextColumn();
            ImGui::TableHeader("IsMainGuid");

            ImGui::EndTable();
        }

        if (ImGui::BeginTable("Content", 3))
        {
            for (auto& [objectRef,objectID] : ObjectRef::GetAll())
            {
                if (FindObjectOfObjectID(objectID) == nullptr)continue;

                ImGui::TableNextColumn();
                ImGui::Text(std::format("{} {}", objectRef.guid.str(), objectRef.fileID).c_str());

                ImGui::TableNextColumn();
                if (ImGui::Button(std::to_string(objectID).c_str()))
                    if (clickObjectEvent != nullptr)
                        clickObjectEvent(FindObjectOfObjectID(objectID));

                ImGui::TableNextColumn();
                bool isMain = ObjectRef::IsRoot(objectRef);
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
