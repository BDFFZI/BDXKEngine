#include "GameObjectEditor.h"

#include "BDXKEngine/Base/Object/Core/ObjectTransferer.h"
#include "BDXKEngine/Framework/Core/GameObject.h"
#include "BDXKEngine/Framework/Core/Component.h"
#include "BDXKEngine/Platform/GUI/GUI.h"
#include "imgui/imgui.h"

namespace BDXKEditor
{
    bool addComponenting;
    std::vector<Reflection*> componentReflections;
    void GameObjectEditor::OnInspectorGUI()
    {
        GUITransferer& gui = GetGUITransferer();
        auto* target = GetTarget().ToObject<GameObject>();
        const Reflection& reflection = Reflection::GetReflection<GameObject>();

        //物体
        {
            bool enable = reflection.GetFieldOf<bool>(target, "isEnabling");
            gui.TransferField("##isEnabling", enable);
            target->SetIsEnabling(enable);
            ImGui::SameLine();
            gui.TransferField("##Name", reflection.GetFieldOf<std::string>(target, "name"));
            ImGui::SameLine();
            if (ImGui::Button(gui.GetFieldID("X").c_str()))
            {
                Object::DestroyImmediate(target);
                return;
            }

            gui.TransferField("Position", reflection.GetFieldOf<Vector3>(target, "localPosition"));
            gui.TransferField("Rotation", reflection.GetFieldOf<Vector3>(target, "localEulerAngles"));
            gui.TransferField("Scale", reflection.GetFieldOf<Vector3>(target, "localScale"));
            target->SetLocalPosition(target->GetLocalPosition());
            target->SetLocalEulerAngles(target->GetLocalEulerAngles());
            target->SetLocalScale(target->GetLocalScale());
        }

        //组件
        {
            auto& components = reflection.GetFieldOf<std::vector<ObjectPtr<Component>>>(target, "components");
            const int count = static_cast<int>(components.size());
            for (int index = 0; index < count; index++)
            {
                ImGui::Separator();

                auto* component = components[index].ToObject<Component>();
                std::string path = "##Component" + std::to_string(index);
                gui.PushPath(path);

                //基本面板
                bool enable = reflection.GetFieldOf<bool>(component, "isEnabling");
                gui.TransferField("##" + std::to_string(component->GetInstanceID()), enable);
                component->SetIsEnabling(enable);
                ImGui::SameLine();
                gui.TransferField("##Name", reflection.GetFieldOf<std::string>(component, "name"));
                ImGui::SameLine();
                if (ImGui::Button(gui.GetFieldID("X").c_str()))
                {
                    Object::DestroyImmediate(component);
                    return;
                }

                //自定义面板
                Editor* editor = GetEditor(*component);
                editor->SetTarget(components[index]);
                editor->SetGui(&gui);
                editor->DrawInspectorGUI();

                gui.PopPath(path);

                ImGui::Separator();
            }
        }

        //添加组件
        {
            if (ImGui::Button("AddComponent", {ImGui::GetContentRegionAvail().x, 0}))addComponenting = true;
            if (addComponenting)
            {
                ImGui::BeginListBox("##AddComponent", {ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y});

                const int count = Reflection::GetReflections(
                    componentReflections,
                    [](const Reflection& reflection) { return reflection.CanConvertTo<Component>(); }
                );

                for (int i = 0; i < count; i++)
                {
                    if (ImGui::Button(componentReflections[i]->GetType().c_str()))
                    {
                        Component::Create(target, componentReflections[i]->GetType());
                        ImGui::EndListBox();
                        return;
                    }
                }

                ImGui::EndListBox();
                if (ImGui::IsWindowHovered(ImGuiHoveredFlags_RootAndChildWindows | ImGuiHoveredFlags_AllowWhenBlockedByActiveItem) == false)
                    addComponenting = false;
            }
        }
    }
}
