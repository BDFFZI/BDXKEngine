#include "HierarchyView.h"
#include "InspectorView.h"
#include "EditorSystem.h"
#include "BDXKEngine/Base/Serialization/Json/JsonWriter.h"


namespace BDXKEditor
{
    void HierarchyView::DrawGameObject(const ObjectPtr<GameObject>& gameObject, float offsetX)
    {
        const int instanceID = gameObject->GetInstanceID();
        const std::string name = gameObject->GetName();
        const ObjectPtr<Transform> transform = gameObject->GetTransform();
        const int childCount = transform->GetChildCount();


        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + offsetX);

        static char checkboxID[30];
        int length = sprintf_s(checkboxID, "##%s_isSpread", name.c_str());
        isSpreads[instanceID] = ImGui::CollapsingHeader(
            checkboxID,
            ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_DefaultOpen | (childCount == 0 ? ImGuiTreeNodeFlags_Leaf : 0));

        //按钮
        ImGui::SameLine();
        ImGui::PushStyleColor(ImGuiCol_Button, ImGui::GetColorU32(ImGuiCol_Button, gameObject->IsActivatingAndEnabling() ? 1 : 0.3f));
        if (ImGui::Button(name.c_str()))
        {
            EditorSystem::SetFocusing(gameObject);
        }
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
                const ObjectPtrBase dropping = *static_cast<ObjectPtrBase*>(payload->Data);
                const ObjectPtr droppingGameObject = dropping.ToObject<GameObject>();
                if (droppingGameObject.IsNotNull())
                    droppingGameObject->GetTransform()->SetParent(transform);
            }
            ImGui::EndDragDropTarget();
        }

        if (isSpreads[instanceID])
        {
            for (int i = 0; i < childCount; i++)
                DrawGameObject(transform->GetChild(i)->GetGameObject(), offsetX + 30);
        }
    }

    void HierarchyView::OnGUI()
    {
        if (ImGui::Button("保存"))
        {
            rapidjson::Document document;
            document.Parse("{}");
            JsonWriter writer(document);

            Serializable* serializable = EditorSystem::GetMainScene().ToObjectBase();
            serializable->Transfer(writer);

            Debug::Log(writer.ToString());
        }
        ImGui::SameLine();
        if (ImGui::Button("加载"))
        {
        }
        ImGui::SameLine();
        ImGui::Button("孤儿箱");
        if (ImGui::BeginDragDropTarget())
        {
            if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("Dragging"))
            {
                ObjectPtrBase dropping = *static_cast<ObjectPtrBase*>(payload->Data);
                const ObjectPtr droppingGameObject = dropping.ToObject<GameObject>();
                if (droppingGameObject.IsNotNull())
                    droppingGameObject->GetTransform()->SetParent(nullptr);
            }
            ImGui::EndDragDropTarget();
        }

        const std::vector<ObjectPtr<Transform>> rootTransforms = TransformManager::GetRootTransforms();
        for (const ObjectPtr<Transform>& transform : rootTransforms)
        {
            DrawGameObject(transform->GetGameObject(), 0);
        }
    }
}
