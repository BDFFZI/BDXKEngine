#pragma once
#include <BDXKEngine/Engine.h>
#include "EditorWindow.h"

namespace BDXKEditor
{
    using namespace BDXKEngine;

    class SceneView : public EditorWindow, public PreRenderFrameHandler, TransformManager, GL
    {
        ObjectPtr<Camera> editorCamera;
        ObjectPtr<Texture2D> editorCameraTexture;
        Vector2 viewSize;

        // ObjectPtr<ObjectSwitchable> focus;
        // int frameRate = 0;

        void OnPreRenderFrame() override;
        void OnDrawWindow() override;
        void OnShow() override;
        
        // float ShowGameObjectInfo(ObjectPtr<GameObject> target, float drawY, int order = 0)
        // {
        //     auto drawX = static_cast<float>(10 + order * 30);
        //     //显示GameObject
        //     {
        //         Rect rect = {drawX, drawY, 160, 25};
        //         GUI::TextArea(rect, target->GetName());
        //         if (Event::IsFocus(rect))
        //             focus = target;
        //     }
        //     //添加Transform事件
        //     const ObjectPtr<Transform> transform = target->GetTransform();
        //     {
        //         Rect transformRect = {drawX + 165, drawY, 160, 20};
        //         //使其可拖拽
        //         if (Event::IsDrag(transformRect, transform))
        //         {
        //             GL2D::DrawRectangleCenter(Input::GetMousePosition(), transformRect.GetSize(), false);
        //         }
        //         //使其可拖入
        //         ObjectPtr<Transform> otherTransform = nullptr;
        //         if (Event::IsDrop(transformRect, &otherTransform))
        //         {
        //             if (otherTransform != nullptr)
        //             {
        //                 try
        //                 {
        //                     otherTransform->SetParent(transform);
        //                     Event::Use();
        //                 }
        //                 catch (std::exception& e)
        //                 {
        //                     Debug::Log(e);
        //                 }
        //             }
        //         }
        //     }
        //     //显示Components
        //     std::vector<ObjectPtr<Component>> components = target->GetComponents();
        //     for (ObjectPtr<Component> component : components)
        //     {
        //         drawX += 165;
        //         Rect rect = {drawX, drawY, 160, 20};
        //
        //         Component* componentPtr = component.ToObject();
        //         String fullName = typeid(*componentPtr).name();
        //         GUI::TextArea(rect, fullName.substr(fullName.find(L':') + 2), 15);
        //
        //         if (Event::IsFocus(rect))focus = component;
        //     }
        //     //显示子物体
        //     if (transform->GetChildCount() != 0)
        //     {
        //         drawY += 30;
        //         for (int index = 0; index < transform->GetChildCount(); index++)
        //         {
        //             drawY = ShowGameObjectInfo(transform->GetChild(index)->GetGameObject(), drawY, order + 1);
        //         }
        //         return drawY;
        //     }
        //
        //     return drawY + 30;
        // }

   
    };
}
