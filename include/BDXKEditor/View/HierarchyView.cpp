#include "HierarchyView.h"
#include "InspectorView.h"
#include "System/EditorSystem.h"


namespace BDXKEditor
{
    void HierarchyView::DrawGameObject(const ObjectPtr<GameObject>& gameObject, float offsetX)
    {
        const int instanceID = gameObject->GetInstanceID();
        const std::string name = gameObject->GetName();
        const ObjectPtr<Transform> transform = gameObject->GetTransform();
        const int childCount = transform->GetChildCount();

        if (isSpreads.contains(instanceID) == false)
            isSpreads[instanceID] = false;


        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + offsetX);
        
        bool isSpread = childCount == 0 ? true : isSpreads[instanceID];
        static char checkboxID[30];
        int length = sprintf_s(checkboxID, "##%s_isSpread", name.c_str());
        if (ImGui::Checkbox(checkboxID, &isSpread))
            isSpreads[instanceID] = isSpread;

        ImGui::SameLine();
        if (ImGui::Button(name.c_str()))
        {
            EditorSystem::inspectorView->target = static_cast<ObjectPtrBase>(gameObject);
        }

        if (isSpreads[instanceID])
        {
            for (int i = 0; i < childCount; i++)
                DrawGameObject(transform->GetChild(i)->GetGameObject(), offsetX + 30);
        }
    }
    
    void HierarchyView::OnDrawWindow()
    {
        const std::vector<ObjectPtr<Transform>> rootTransforms = TransformManager::GetRootTransforms();
        for (const ObjectPtr<Transform>& transform : rootTransforms)
        {
            DrawGameObject(transform->GetGameObject(), 0);
        }
    }
}
