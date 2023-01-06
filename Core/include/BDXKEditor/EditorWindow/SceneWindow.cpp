#include "SceneWindow.h"
#include "BDXKEngine/Framework/Scene.h"
#include "BDXKEngine/Function/Time/Time.h"
#include "BDXKEngine/Platform/GUI/GUI.h"
#include "ImGuizmo/ImGuizmo.h"
#include "BDXKEngine/Function/Window/Cursor.h"
#include "BDXKEngine/Platform/Window/Window.h"

namespace BDXKEditor
{
    ImGuizmo::OPERATION handleOption;

    bool CameraController::IsControlling() const
    {
        return isControlling;
    }
    void CameraController::OnStart()
    {
        transform = GetGameObject();
    }
    void CameraController::OnUpdate()
    {
        if (ImGui::IsMouseClicked(ImGuiMouseButton_Right))
        {
            Cursor::SetLockState(true);
            Cursor::SetVisible(false);
        }
        if (ImGui::IsMouseReleased(ImGuiMouseButton_Right))
        {
            Cursor::SetLockState(false);
            Cursor::SetVisible(true);
        }
        if ((isControlling = ImGui::IsMouseDown(ImGuiMouseButton_Right)))
        {
            const float deltaTime = Time::GetDeltaTime();

            //位置调整
            Vector3 position = transform->GetLocalPosition();
            if (ImGui::IsKeyDown(ImGuiKey_W))
            {
                position += transform->GetFront().GetNormalized() * deltaTime * 4;
            }
            if (ImGui::IsKeyDown(ImGuiKey_S))
            {
                position += -transform->GetFront().GetNormalized() * deltaTime * 4;
            }
            if (ImGui::IsKeyDown(ImGuiKey_A))
            {
                position += -transform->GetRight().GetNormalized() * deltaTime * 4;
            }
            if (ImGui::IsKeyDown(ImGuiKey_D))
            {
                position += transform->GetRight().GetNormalized() * deltaTime * 4;
            }
            if (ImGui::IsKeyDown(ImGuiKey_Q))
            {
                position += -transform->GetUp().GetNormalized() * deltaTime * 4;
            }
            if (ImGui::IsKeyDown(ImGuiKey_E))
            {
                position += transform->GetUp().GetNormalized() * deltaTime * 4;
            }
            transform->SetLocalPosition(position);

            //视角调整
            Vector3 localEulerAngles = transform->GetLocalEulerAngles();
            const Vector2 mouseMoveDelta = Window::GetCursorMoveDelta();
            localEulerAngles.y += mouseMoveDelta.x * deltaTime * 5;
            localEulerAngles.x += mouseMoveDelta.y * deltaTime * 5;
            transform->SetLocalEulerAngles(localEulerAngles);
        }
    }

    void SceneWindow::SetTarget(const ObjectPtr<GameObject>& target)
    {
        this->target = target;
    }
    void SceneWindow::OnAwake()
    {
        const ObjectPtr<GameObject> editorCameraGameObject = GameObject::Create("SceneWindow");
        Component::Create<Camera>(editorCameraGameObject);
        GameObject::Hide(editorCameraGameObject);

        camera = editorCameraGameObject->GetComponent<Camera>();
        cameraTexture = Texture2D::Create(960, 540, TextureFormat::R8G8B8A8_UNORM);
        cameraController = Component::Create<CameraController>(editorCameraGameObject);
        viewSize = {960, 540};

        camera->SetRenderTarget(cameraTexture);
        camera->SetBackground(Color::gray * 0.5f);
    }
    void SceneWindow::OnGUI()
    {
        //更新渲染纹理大小
        const Vector2 windowMin = ImGui::GetWindowContentRegionMin();
        const Vector2 windowMax = ImGui::GetWindowContentRegionMax();
        viewSize = windowMax - windowMin;
        if (cameraTexture->GetSize() != viewSize && viewSize.x > 0 && viewSize.y > 0)
        {
            cameraTexture = Texture2D::Create(viewSize.GetXInt(), viewSize.GetYInt(), TextureFormat::R8G8B8A8_UNORM);
            camera->SetRenderTarget(cameraTexture);
        }
        //控制场景相机
        cameraController->SetIsEnabling(ImGui::IsWindowHovered());

        //相机画面
        const Vector2 cursorPos = ImGui::GetCursorPos();
        ImGui::Image(
            GUI::GetImTextureID(cameraTexture),
            cameraTexture->GetSize()
        );
        ImGui::SetCursorPos(cursorPos);
        //手柄选项
        {
            static constexpr char optionsName[4][10] = {"Hide", "Position", "Rotation", "Scale"};
            static constexpr ImGuizmo::OPERATION options[] = {ImGuizmo::BOUNDS, ImGuizmo::TRANSLATE, ImGuizmo::ROTATE, ImGuizmo::SCALE};
            const float width = ImGui::GetContentRegionAvail().x / 4 - 10;
            static int handleMode = 1;
            for (int i = 0; i < 4; i++)
            {
                ImGui::SetCursorPos(cursorPos + Vector2{(width + 10) * static_cast<float>(i), 0.0f});
                if (ImGui::Selectable(optionsName[i], handleMode == i, 0, Vector2{width, 0.0f}))
                    handleMode = i;
            }
            if (ImGui::IsWindowHovered() && cameraController->IsControlling() == false)
            {
                if (ImGui::IsKeyDown(ImGuiKey_Q))handleMode = 0;
                if (ImGui::IsKeyDown(ImGuiKey_W))handleMode = 1;
                if (ImGui::IsKeyDown(ImGuiKey_E))handleMode = 2;
                if (ImGui::IsKeyDown(ImGuiKey_R))handleMode = 3;
            }
            handleOption = options[handleMode];
        }
        //帧率信息
        ImGui::Text(
            "Rate %.3f ms/frame (%.1f FPS)",
            static_cast<double>(Time::GetDeltaTime() * 1000.0f),
            static_cast<double>(1.0f / Time::GetDeltaTime())
        );

        //OnDrawGizmos
        if (GUI::IsDockTabVisible())
        {
            const Vector2 viewPosition = Vector2{ImGui::GetWindowPos()} + windowMin;
            ImGuizmo::SetRect(viewPosition.x, viewPosition.y, viewSize.x, viewSize.y);
            ImGuizmo::SetOrthographic(false);
            CameraInfo cameraInfo = camera->GetCameraInfo();
            //绘制网格
            if (handleOption != ImGuizmo::BOUNDS)
            {
                Matrix4x4 objectToWorld = Matrix4x4::identity;
                ImGuizmo::DrawGrid(
                    reinterpret_cast<float*>(&cameraInfo.worldToView),
                    reinterpret_cast<float*>(&cameraInfo.viewToClip),
                    reinterpret_cast<float*>(&objectToWorld),
                    10
                );
            }
            //绘制手柄
            if (target.IsNotNull())
            {
                Matrix4x4 objectToWorld = target->GetLocalToWorldMatrix();
                Manipulate(
                    reinterpret_cast<float*>(&cameraInfo.worldToView),
                    reinterpret_cast<float*>(&cameraInfo.viewToClip),
                    handleOption, ImGuizmo::LOCAL,
                    reinterpret_cast<float*>(&objectToWorld)
                );

                Vector3 position;
                Vector3 rotation;
                Vector3 scale;
                ObjectPtr<GameObject> parent = target->GetParent();
                Matrix4x4::DecomposeTRS(
                    parent.IsNull() ? objectToWorld : parent->GetWorldToLocalMatrix() * objectToWorld,
                    position, rotation, scale
                );
                target->SetLocalPosition(position);
                target->SetLocalEulerAngles(rotation);
                target->SetLocalScale(scale);
            }
        }
    }
}
