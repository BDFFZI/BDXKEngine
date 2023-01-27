#include "SceneWindow.h"
#include "InspectorWindow.h"
#include "BDXKEngine/Framework/Scene.h"
#include "BDXKEngine/Function/Time/Time.h"
#include "BDXKEngine/Function/Window/Cursor.h"
#include "BDXKEngine/Platform/Window/Window.h"
#include "BDXKEngine/Platform/GUI/GUI.h"
#include <ImGuizmo/ImGuizmo.h>

namespace BDXKEditor
{
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
            const auto moveSpeed = static_cast<float>(4 * (ImGui::IsKeyDown(ImGuiKey_LeftShift) ? 3 : 1));

            //位置调整
            Vector3 position = transform->GetLocalPosition();
            if (ImGui::IsKeyDown(ImGuiKey_W))
            {
                position += transform->GetFront().GetNormalized() * deltaTime * moveSpeed;
            }
            if (ImGui::IsKeyDown(ImGuiKey_S))
            {
                position += -transform->GetFront().GetNormalized() * deltaTime * moveSpeed;
            }
            if (ImGui::IsKeyDown(ImGuiKey_A))
            {
                position += -transform->GetRight().GetNormalized() * deltaTime * moveSpeed;
            }
            if (ImGui::IsKeyDown(ImGuiKey_D))
            {
                position += transform->GetRight().GetNormalized() * deltaTime * moveSpeed;
            }
            if (ImGui::IsKeyDown(ImGuiKey_Q))
            {
                position += -transform->GetUp().GetNormalized() * deltaTime * moveSpeed;
            }
            if (ImGui::IsKeyDown(ImGuiKey_E))
            {
                position += transform->GetUp().GetNormalized() * deltaTime * moveSpeed;
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

    bool SceneWindow::HasMenu()
    {
        return true;
    }

    void SceneWindow::OnAwake()
    {
        gameObject = GameObject::Create("BDXKEditor::SceneWindow");
        Component::Create<Camera>(gameObject);
        GameObject::Hide(gameObject);

        editorCamera = gameObject->GetComponent<Camera>();
        cameraTexture = Texture2D::Create(960, 540, TextureFormat::R8G8B8A8_UNORM);
        cameraController = Component::Create<CameraController>(gameObject);
        viewSize = {960, 540};

        editorCamera->SetRenderTarget(cameraTexture);
        editorCamera->SetBackground(Color::gray * 0.5f);
    }
    void SceneWindow::OnGUI()
    {
        //控制场景相机
        cameraController->SetIsEnabling(ImGui::IsWindowHovered());
        if (cameraOption == 0)
        {
            const auto* editorCameraPtr = editorCamera.ToObject<Camera>();
            for (auto& item : Camera::GetCameraQueue())
            {
                if (item != editorCameraPtr)
                {
                    editorCamera->SetBackground(item->GetBackground());
                    editorCamera->SetClearFlags(item->GetClearFlags());
                }
            }
        }
        //更新渲染纹理大小
        const Vector2 windowMin = ImGui::GetWindowContentRegionMin();
        const Vector2 windowMax = ImGui::GetWindowContentRegionMax();
        viewSize = windowMax - windowMin;
        if (cameraTexture->GetSize() != viewSize && viewSize.x > 0 && viewSize.y > 0)
        {
            cameraTexture = Texture2D::Create(viewSize.GetXInt(), viewSize.GetYInt(), TextureFormat::R8G8B8A8_UNORM);
            editorCamera->SetRenderTarget(cameraTexture);
        }

        if (ImGui::BeginMenuBar())
        {
            //手柄选项
            {
                ImGui::SetNextItemWidth(viewSize.x * 0.125f);
                static const char* optionName[] = {"Hide", "Position", "Rotation", "Scale"};
                ImGui::Combo("Handle", &handleOption, optionName, 4);
                if (ImGui::IsWindowHovered() && cameraController->IsControlling() == false)
                {
                    if (ImGui::IsKeyPressed(ImGuiKey_Q))handleOption = 0;
                    if (ImGui::IsKeyPressed(ImGuiKey_W))handleOption = 1;
                    if (ImGui::IsKeyPressed(ImGuiKey_E))handleOption = 2;
                    if (ImGui::IsKeyPressed(ImGuiKey_R))handleOption = 3;
                }
            }

            //编辑器相机设置
            {
                ImGui::SetNextItemWidth(viewSize.x * 0.15f);
                static const char* optionName[] = {"Game Camera", "Custom"};
                ImGui::Combo("Camera", &cameraOption, optionName, 2);
                if (cameraOption == 1 && ImGui::MenuItem("Editor Camera"))
                {
                    ObjectPtr<InspectorWindow> inspectorWindow = Create<InspectorWindow>();
                    inspectorWindow->SetTarget(gameObject);
                    inspectorWindow->Show();
                }
            }

            ImGui::EndMenuBar();
        }

        //显示相机画面
        const Vector2 cursorPos = ImGui::GetCursorPos();
        ImGui::Image(
            GUI::GetImTextureID(cameraTexture),
            cameraTexture->GetSize()
        );
        ImGui::SetCursorPos(cursorPos);

        //帧率信息
        static double deltaTime = 0;
        deltaTime = std::lerp(deltaTime, Time::GetDeltaTime(), 0.05f);
        ImGui::TextColored(
            Color::magenta,
            "Frame Rate %5.1f ms/frame (%5.1f FPS)",
            deltaTime * 1000.0,
            1.0 / deltaTime
        );

        //DrawSceneGUI回调
        if (target != nullptr)
        {
            Editor::GetEditor(target).DrawSceneGUI();
        }

        //OnDrawGizmos
        if (GUI::IsDockTabVisible() && handleOption != 0)
        {
            const Vector2 viewPosition = Vector2{ImGui::GetWindowPos()} + windowMin;
            ImGuizmo::SetRect(viewPosition.x, viewPosition.y, viewSize.x, viewSize.y);
            ImGuizmo::SetOrthographic(false);
            CameraInfo cameraInfo = editorCamera->GetCameraInfo();

            //绘制网格
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
                static constexpr ImGuizmo::OPERATION options[] = {ImGuizmo::BOUNDS, ImGuizmo::TRANSLATE, ImGuizmo::ROTATE, ImGuizmo::SCALE};
                ImGuizmo::OPERATION imguiOption = options[handleOption];

                Matrix4x4 objectToWorld = target->GetLocalToWorldMatrix();
                Manipulate(
                    reinterpret_cast<float*>(&cameraInfo.worldToView),
                    reinterpret_cast<float*>(&cameraInfo.viewToClip),
                    imguiOption, ImGuizmo::LOCAL,
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
