#include "GameObjectEditor.h"
#include "BDXKEngine/Framework/Core/GameObject.h"
#include "BDXKEngine/Framework/Core/Component.h"
#include "imgui/imgui.h"

namespace BDXKEditor
{
    void GameObjectEditor::OnInspectorGUI()
    {
        Transferer* gui = GetGui();
        auto* target = GetTarget().ToObject<GameObject>();
        const Reflection reflection = Reflection::GetReflection<GameObject>();

        //物体
        {
            bool enable = reflection.GetFieldOf<bool>(target, "isEnabling");
            gui->TransferField("##isEnabling", enable);
            target->SetIsEnabling(enable);
            ImGui::SameLine();
            gui->TransferField("Name", reflection.GetFieldOf<std::string>(target, "name"));

            gui->TransferField("Position", reflection.GetFieldOf<Vector3>(target, "localPosition"));
            gui->TransferField("Rotation", reflection.GetFieldOf<Vector3>(target, "localEulerAngles"));
            gui->TransferField("Scale", reflection.GetFieldOf<Vector3>(target, "localScale"));
            target->SetParent(target->GetParent());
        }

        //组件
        {
            const auto& components = reflection.GetFieldOf<std::vector<ObjectPtr<Component>>>(target, "components");
            const int count = static_cast<int>(components.size());
            for (int index = 0; index < count; index++)
            {
                auto* component = components[index].ToObject<Component>();
                std::string path = "Component " + std::to_string(index);
                gui->PushPath(path);
                ImGui::Separator();
                ImGui::Separator();

                //基本面板
                bool enable = reflection.GetFieldOf<bool>(component, "isEnabling");
                gui->TransferField("##" + std::to_string(component->GetInstanceID()), enable);
                component->SetIsEnabling(enable);
                ImGui::SameLine();
                gui->TransferField("Name", reflection.GetFieldOf<std::string>(component, "name"));

                //自定义面板
                Editor* editor = GetEditor(component->GetType());
                editor->SetTarget(components[index]);
                editor->SetGui(gui);
                editor->DrawInspectorGUI();

                gui->PopPath(path);
            }
        }
    }
}
