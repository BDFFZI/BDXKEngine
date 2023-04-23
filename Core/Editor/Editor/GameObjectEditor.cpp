﻿#include "GameObjectEditor.h"
#include "Core/Runtime/Base/Object/Transferer/ObjectTransferer.h"
#include "Core/Runtime/Framework/Actor/GameObject.h"
#include "Core/Runtime/Framework/Actor/Component.h"
#include "Core/Runtime/Function/GUI/GUI.h"

namespace BDXKEditor
{
    bool addComponenting;
    std::vector<Reflection*> componentReflections;

    bool GameObjectEditor::DrawTitle(SwitchableObject* target) const
    {
        //类型
        ImGui::TextDisabled(target->GetType().c_str());
        //实例编号
        ImGui::SameLine();
        ImGui::Button(std::to_string(target->GetObjectID()).c_str());
        GUI::IsDragSource(target);

        const Reflection& reflection = Reflection::GetReflection<SwitchableObject>();
        GUITransferer& gui = GetGUI();
        //是否启动
        bool enable = reflection.GetFieldOf<bool>(target, "isEnabling");
        gui.TransferField("##IsEnabling", enable);
        target->SetIsEnabling(enable);
        //名称
        ImGui::SameLine();
        gui.TransferField("##Name", reflection.GetFieldOf<std::string>(target, "name"));
        //删除功能
        ImGui::SameLine();
        if (ImGui::Button(gui.GetFieldID("X").c_str()))
        {
            Object::DestroyObject(target);
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
                std::string path = "##Component" + std::to_string(component.GetObjectID());
                gui.PushPath(path);
                ImGui::Separator();

                if (DrawTitle(component.ToObject<SwitchableObject>()))
                {
                    GetEditor(component, gui)->DrawInspectorGUI();
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
