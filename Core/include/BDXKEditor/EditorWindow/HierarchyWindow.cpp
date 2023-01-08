#include "HierarchyWindow.h"
#include <format>
#include "BDXKEngine/Platform/GUI/GUI.h"

namespace BDXKEditor
{
    void HierarchyWindow::SetClickGameObjectEvent(const std::function<void(const ObjectPtr<GameObject>&)>& clickGameObjectEvent)
    {
        this->clickGameObjectEvent = clickGameObjectEvent;
    }
    void HierarchyWindow::DrawGameObject(const ObjectPtr<GameObject>& gameObject, float offsetX)
    {
        const ID instanceID = gameObject->GetInstanceID();
        const std::string name = gameObject->GetName();
        const int childCount = gameObject->GetChildCount();

        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + offsetX);

        //下拉按钮
        ImGui::PushStyleColor(ImGuiCol_Header, ImGui::GetColorU32(ImGuiCol_Header, 0));
        isUnfolding[instanceID] = ImGui::CollapsingHeader(
            std::format("##{}_isSpread", instanceID).c_str(),
            ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_DefaultOpen | (childCount == 0 ? ImGuiTreeNodeFlags_Leaf : 0));
        ImGui::PopStyleColor();
        //选择物体按钮
        ImGui::SameLine();
        ImGui::PushStyleColor(ImGuiCol_Button, ImGui::GetColorU32(ImGuiCol_Button, gameObject->IsActivatingAndEnabling() ? 1 : 0.5f));
        ImGui::PushStyleVar(ImGuiStyleVar_ButtonTextAlign, {0, 0.5f});
        if (ImGui::Button(std::format("{}##{}", name, instanceID).c_str(), {ImGui::GetContentRegionAvail().x, 0})
            && clickGameObjectEvent != nullptr
        )
            clickGameObjectEvent(gameObject);
        ImGui::PopStyleVar();
        ImGui::PopStyleColor();
        //拖拽功能
        if (ImGui::BeginDragDropSource())
        {
            ImGui::SetDragDropPayload("Dragging", &gameObject, sizeof(ObjectPtrBase));
            ImGui::EndDragDropSource();
        }
        if (ImGui::BeginDragDropTarget())
        {
            if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("Dragging"))
            {
                const ObjectPtrBase& dropping = *static_cast<ObjectPtrBase*>(payload->Data);
                const ObjectPtr droppingGameObject = dropping.ToObject<GameObject>();
                if (droppingGameObject.IsNotNull())
                    droppingGameObject->SetParent(gameObject);
            }
            ImGui::EndDragDropTarget();
        }

        if (isUnfolding[instanceID])
        {
            for (int i = 0; i < childCount; i++)
                DrawGameObject(gameObject->GetChild(i), offsetX + 30);
        }
    }

    void HierarchyWindow::OnGUI()
    {
        if (ObjectPtrBase objectPtr = {}; GUI::IsDragTargetForWindow("##HierarchyWindow", objectPtr))
        {
            const ObjectPtr gameObject = objectPtr.ToObject<GameObject>();
            if (gameObject != nullptr)gameObject->SetParent(nullptr);
        }

        const std::vector<ObjectPtr<GameObject>> gameObjects = GameObject::GetGameObjects();
        for (const ObjectPtr<GameObject>& gameObject : gameObjects)
        {
            if (gameObject->GetParent().IsNull())
                DrawGameObject(gameObject, 0);
        }
    }
}
