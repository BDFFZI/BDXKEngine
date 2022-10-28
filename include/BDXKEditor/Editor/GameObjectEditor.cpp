#include "GameObjectEditor.h"

#include "BDXKEditor/EditorSystem.h"
#include "BDXKEngine/Framework/GameObject/GameObject.h"
#include "BDXKEngine/Framework/Component/Component.h"
#include "imgui/imgui.h"

namespace BDXKEditor
{
    void GameObjectEditor::OnInspectorGUI()
    {
        const ObjectPtr gameObject = target.ToObject<GameObject>();
        gameObject->Transfer(*gui);

        const int count = static_cast<int>(gameObject->components.size());
        for (int index = 0; index < count; index++)
        {
            std::string key = "component_" + std::to_string(index);
            gui->PushPath(key);
            ImGui::Separator();
            gameObject->components[index]->Transfer(*gui);
            ImGui::Separator();
            gui->PopPath(key);

            // //获取组件信息
            // ObjectPtr<Component> component = gameObject->components[index];
            // const int instanceID = component.GetInstanceID();
            // const std::string typeID = component->GetTypeID();
            // //获取组件编辑器
            // ObjectPtr<Editor> editor = editors[instanceID];
            // if (editor.IsNull())
            // {
            //     editor = editors[instanceID] = EditorSystem::GetEditor(typeID);
            //     editor->SetTarget(component);
            //     editor->SetGui(*gui);
            //     editor->SetIsEnabling(true);
            // }
            // //绘制
            // ImGui::Separator();
            // ImGui::LabelText(
            //     ("component" + std::to_string(instanceID)).c_str(),
            //     (typeID + " " + std::to_string(instanceID)).c_str()
            // );
            // editor->OnInspectorGUI();
            // ImGui::Separator();
        }
    }
}
