#include "HierarchyWindow.h"
#include "imgui/imgui.h"


namespace BDXKEditor
{
    char checkboxID[128];

    void HierarchyWindow::SetClickGameObjectEvent(const std::function<void(const ObjectPtr<GameObject>&)>& clickGameObjectEvent)
    {
        this->clickGameObjectEvent = clickGameObjectEvent;
    }
    void HierarchyWindow::DrawGameObject(const ObjectPtr<GameObject>& gameObject, float offsetX)
    {
        const int instanceID = gameObject->GetInstanceID();
        const std::string name = gameObject->GetName();
        const int childCount = gameObject->GetChildCount();


        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + offsetX);
        sprintf_s(checkboxID, "##%s_isSpread", name.c_str());
        isSpreads[instanceID] = ImGui::CollapsingHeader(
            checkboxID,
            ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_DefaultOpen | (childCount == 0 ? ImGuiTreeNodeFlags_Leaf : 0));

        //按钮
        ImGui::SameLine();
        ImGui::PushStyleColor(ImGuiCol_Button, ImGui::GetColorU32(ImGuiCol_Button, gameObject->IsActivatingAndEnabling() ? 1 : 0.3f));
        if (ImGui::Button((name + "##" + std::to_string(instanceID)).c_str()) && clickGameObjectEvent != nullptr)
            clickGameObjectEvent(gameObject);
        ImGui::PopStyleColor();

        //拖拽
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

        if (isSpreads[instanceID])
        {
            for (int i = 0; i < childCount; i++)
                DrawGameObject(gameObject->GetChild(i), offsetX + 30);
        }
    }

    void HierarchyWindow::OnGUI()
    {
        ImGui::Button("Isolate");
        if (ImGui::BeginDragDropTarget())
        {
            if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("Dragging"))
            {
                const ObjectPtrBase& dropping = *static_cast<ObjectPtrBase*>(payload->Data);
                const ObjectPtr droppingGameObject = dropping.ToObject<GameObject>();
                if (droppingGameObject.IsNotNull())
                    droppingGameObject->SetParent(nullptr);
            }
            ImGui::EndDragDropTarget();
        }
        ImGui::SameLine();
        if (ImGui::Button("New GameObject"))
        {
            GameObject::Create();
            return;
        }

        const std::vector<ObjectPtr<GameObject>> gameObjects = GameObject::GetGameObjects();
        for (const ObjectPtr<GameObject>& gameObject : gameObjects)
        {
            if (gameObject->GetParent().IsNull())
                DrawGameObject(gameObject, 0);
        }
    }
}
