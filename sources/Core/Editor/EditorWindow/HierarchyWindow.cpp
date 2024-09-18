#include "HierarchyWindow.h"
#include <format>
#include "Core/Runtime/Function/GUI/GUI.h"

namespace BDXKEditor
{
    void HierarchyWindow::SetClickObjectEvent(const std::function<void(const ObjectPtrBase&)>& clickGameObjectEvent)
    {
        this->clickObjectEvent = clickGameObjectEvent;
    }
    void HierarchyWindow::DrawGameObject(const ObjectPtr<GameObject>& gameObject, const float offsetX)
    {
        const ID objectID = gameObject->GetObjectID();
        const std::string name = gameObject->GetName();
        const int childCount = gameObject->GetChildCount();

        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + offsetX);

        //下拉按钮
        ImGui::PushStyleColor(ImGuiCol_Header, ImGui::GetColorU32(ImGuiCol_Header, 0));
        isUnfolding[objectID] = ImGui::CollapsingHeader(
            std::format("##{}_isSpread", objectID).c_str(),
            ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_DefaultOpen | (childCount == 0 ? ImGuiTreeNodeFlags_Leaf : 0));
        ImGui::PopStyleColor();
        //选择物体按钮
        ImGui::SameLine();
        ImGui::PushStyleColor(ImGuiCol_Button, ImGui::GetColorU32(ImGuiCol_Button, gameObject->IsActivatingAndEnabling() ? 1 : 0.5f));
        DrawObject(gameObject);
        ImGui::PopStyleColor();
        //拖拽设置父级
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

        if (isUnfolding[objectID])
        {
            for (int i = 0; i < childCount; i++)
                DrawGameObject(gameObject->GetChild(i), offsetX + 30);
        }
    }
    void HierarchyWindow::DrawObject(const ObjectPtrBase& object) const
    {
        ImGui::PushStyleVar(ImGuiStyleVar_ButtonTextAlign, {0, 0.5f});
        if (ImGui::Button(std::format("{}##{}", object->GetName(), object.GetObjectID()).c_str(), {ImGui::GetContentRegionAvail().x, 0})
            && clickObjectEvent != nullptr
        )
            clickObjectEvent(object);
        ImGui::PopStyleVar();
    }
    void HierarchyWindow::DrawScene(const ObjectPtr<Scene>& scene)
    {
        ImGui::Text(scene->GetName().c_str());
        for (size_t i = 0; i < scene->GetAllObjects().size(); i++) // NOLINT(modernize-loop-convert) 别睬他，因为调整场景时数组会变，会报错
        {
            const ObjectPtrBase& object = scene->GetAllObjects()[i];
            ObjectPtr gameObject = object.ToObject<GameObject>();
            if (gameObject.IsNotNull())
            {
                if (gameObject->GetParent().IsNull())
                    DrawGameObject(gameObject, 0);
            }
            else if (object.IsNotNull())
                DrawObject(object);
            else
                ImGui::Text("物体销毁异常");
        }
    }

    void HierarchyWindow::OnGUI()
    {
        //点击取消物体选择
        if (GUI::ButtonWindow())
        {
            clickObjectEvent(nullptr);
        }
        //拖拽置空父级
        if (ObjectPtrBase objectPtr = {}; GUI::IsDragTarget(objectPtr))
        {
            ObjectPtr gameObject = objectPtr.ToObject<GameObject>();
            if (gameObject != nullptr)
            {
                if (ObjectSerialization::IsPersistent(gameObject) &&
                    ObjectSerialization::GetPersistent(gameObject) != ObjectSerialization::GetPersistent(Scene::GetActiveScene()))
                {
                    //拖入外部物体，需要复制 TODO 这是一个临时修复
                    gameObject = ObjectSerialization::Clone<GameObject>(gameObject);
                    gameObject->SetScene(Scene::GetActiveScene());
                }

                gameObject->SetParent(nullptr);
            }
            return;
        }

        DrawScene(Scene::GetActiveScene());
        DrawScene(Scene::GetDurableScene());
    }
}
