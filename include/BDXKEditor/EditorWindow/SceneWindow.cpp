﻿#include "SceneWindow.h"
#include "CreationView.h"
#include "BDXKEditor/EditorSystem.h"

namespace BDXKEditor
{
    ObjectPtr<Camera> SceneWindow::GetEditorCamera() const
    {
        return editorCamera;
    }


    void SceneWindow::OnAwake()
    {
        const ObjectPtr<GameObject> editorCameraGameObject = CreationWindow::Camera("EditorCamera");
        editorCameraGameObject->AddComponent<CameraController>();

        editorCamera = editorCameraGameObject->GetComponent<Camera>();
        editorCameraTexture = Texture2D::Create(960, 540);
        viewSize = {960, 540};

        editorCamera->SetRenderTarget(editorCameraTexture);
    }
    void SceneWindow::OnPreRenderFrame()
    {
        if (editorCameraTexture->GetSize() != viewSize && viewSize.x * viewSize.y > 0)
        {
            editorCameraTexture = Texture2D::Create(viewSize.GetXInt(), viewSize.GetYInt());
            editorCamera->SetRenderTarget(editorCameraTexture);
        }
    }
    void SceneWindow::OnGUI()
    {
        const Vector2 windowMin = ImGui::GetWindowContentRegionMin();
        const Vector2 windowMax = ImGui::GetWindowContentRegionMax();
        const Vector2 viewPosition = Vector2{ImGui::GetWindowPos()} + windowMin;
        viewSize = windowMax - windowMin;

        //显示相机画面
        ImGui::Image(
            IMGUIManager::GetImTextureID(editorCameraTexture),
            editorCameraTexture->GetSize()
        );
        //绘制小物件
        {
            CameraInfo cameraInfo = EditorSystem::GetSceneView()->GetEditorCamera()->GetCameraInfo();
            ImGuizmo::SetRect(viewPosition.x, viewPosition.y, viewSize.x, viewSize.y);
            ImGuizmo::SetOrthographic(false);
            //绘制网格
            Matrix4x4 objectToWorld = Matrix4x4::identity;
            ImGuizmo::DrawGrid(
                reinterpret_cast<float*>(&cameraInfo.worldToView),
                reinterpret_cast<float*>(&cameraInfo.viewToClip),
                reinterpret_cast<float*>(&objectToWorld),
                10
            );
            //绘制手柄
            if (const ObjectPtr<GameObject> gameObject = EditorSystem::GetFocusing().ToObject<GameObject>(); gameObject.IsNotNull())
            {
                const ObjectPtr<Transform> transform = gameObject->GetTransform();
                objectToWorld = transform->GetLocalToWorldMatrix();
                ImGuizmo::Manipulate(
                    reinterpret_cast<float*>(&cameraInfo.worldToView),
                    reinterpret_cast<float*>(&cameraInfo.viewToClip),
                    ImGuizmo::TRANSLATE, ImGuizmo::LOCAL,
                    reinterpret_cast<float*>(&objectToWorld)
                );
                //transform->SetLocalPosition(static_cast<Vector3>(objectToWorld.GetColumn(3)));
            }
        }
        //绘制帧率信息
        ImGui::SameLine(-1);
        ImGui::Text(
            " 平均帧率 %.3f ms/frame (%.1f FPS)",
            static_cast<double>(1000.0f / ImGui::GetIO().Framerate),
            static_cast<double>(ImGui::GetIO().Framerate)
        );
    }
}