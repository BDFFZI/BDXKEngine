#include "GameObjectEditor.h"
#include "BDXKEngine/Base/Object/Transferer/ObjectTransferer.h"
#include "BDXKEngine/Framework/Core/GameObject.h"
#include "BDXKEngine/Framework/Core/Component.h"
#include "BDXKEngine/Platform/GUI/GUI.h"
#include "imgui/imgui.h"

namespace BDXKEditor
{
    bool addComponenting;
    std::vector<Reflection*> componentReflections;

    bool GameObjectEditor::DrawTitle(ScriptableObject* target) const
    {
        //实例编号
        ImGui::Button(std::to_string(target->GetInstanceID()).c_str());
        GUI::IsDragSource(target);
        //类型
        ImGui::SameLine();
        ImGui::TextDisabled(target->GetType().c_str());

        const Reflection& reflection = Reflection::GetReflection<ScriptableObject>();
        GUITransferer& gui = GetGUI();
        //是否启动
        bool enable = reflection.GetFieldOf<bool>(target, "isEnabling");
        gui.TransferField("##isEnabling", enable);
        target->SetIsEnabling(enable);
        //删除功能
        ImGui::SameLine();
        if (ImGui::Button(gui.GetFieldID("X").c_str()))
        {
            Object::DestroyImmediate(target);
            return false;
        }

        return true;
    }
    void GameObjectEditor::OnInspectorGUI() const
    {
        GUITransferer& gui = GetGUI();

        auto* gameObject = GetTarget().ToObject<GameObject>();
        if (DrawTitle(gameObject))
        {
            const Reflection& reflection = Reflection::GetReflection<GameObject>();
            gui.TransferField("Name", reflection.GetFieldOf<std::string>(gameObject, "name"));
            gui.TransferField("Layer", reflection.GetFieldOf<int>(gameObject, "layer"));
            gui.TransferField("Position", reflection.GetFieldOf<Vector3>(gameObject, "localPosition"));
            gui.TransferField("Rotation", reflection.GetFieldOf<Vector3>(gameObject, "localEulerAngles"));
            gui.TransferField("Scale", reflection.GetFieldOf<Vector3>(gameObject, "localScale"));
            gameObject->SetLocalPosition(gameObject->GetLocalPosition());
            gameObject->SetLocalEulerAngles(gameObject->GetLocalEulerAngles());
            gameObject->SetLocalScale(gameObject->GetLocalScale());


            //组件
            for (auto& component : gameObject->GetComponents())
            {
                std::string path = "##Component" + std::to_string(component.GetInstanceID());
                gui.PushPath(path);
                ImGui::Separator();

                if (DrawTitle(component.ToObject<ScriptableObject>()))
                {
                    GetEditor(component, gui).DrawInspectorGUI();
                }

                ImGui::Separator();
                gui.PopPath(path);
            }

            //添加组件
            GUI::Dropdown("AddComponent", [&]
            {
                const int count = Reflection::GetReflections(
                    componentReflections,
                    [](const Reflection& reflection) { return reflection.CanConvertTo<Component>(); }
                );

                for (int i = 0; i < count; i++)
                {
                    if (ImGui::Button(componentReflections[i]->GetType().c_str()))
                    {
                        Component::Create(gameObject, componentReflections[i]->GetType());
                    }
                }
            });
        }
    }
}
