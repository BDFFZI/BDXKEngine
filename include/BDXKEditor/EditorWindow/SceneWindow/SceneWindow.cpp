#include "SceneWindow.h"
#include "CameraController.h"
#include "BDXKEngine/Framework/Scene.h"
#include "BDXKEngine/Function/Time/Time.h"
#include "BDXKEngine/Function/Window/Input.h"
#include "BDXKEngine/Platform/GUI/GUI.h"
#include "ImGuizmo/ImGuizmo.h"

namespace BDXKEditor
{
    void SceneWindow::SetTarget(const ObjectPtr<GameObject>& target)
    {
        this->target = target;
    }

    void SceneWindow::OnAwake()
    {
        const ObjectPtr<GameObject> editorCameraGameObject = Creation::CreateCamera("EditorCamera");
        Component::Create<CameraController>(editorCameraGameObject);

        camera = editorCameraGameObject->GetComponent<Camera>();
        cameraTexture = Texture2D::Create(960, 540);
        viewSize = {960, 540};

        camera->SetRenderTarget(cameraTexture);
    }

    void SceneWindow::OnGUI()
    {
        //更新渲染纹理大小
        if (cameraTexture->GetSize() != viewSize && viewSize.x * viewSize.y > 0)
        {
            cameraTexture = Texture2D::Create(viewSize.GetXInt(), viewSize.GetYInt());
            camera->SetRenderTarget(cameraTexture);
        }

        const Vector2 windowMin = ImGui::GetWindowContentRegionMin();
        const Vector2 windowMax = ImGui::GetWindowContentRegionMax();
        const Vector2 viewPosition = Vector2{ImGui::GetWindowPos()} + windowMin;
        viewSize = windowMax - windowMin;

        //显示相机画面
        const Vector2 cursorPos = ImGui::GetCursorPos();
        ImGui::Image(
            GUI::GetImTextureID(cameraTexture),
            cameraTexture->GetSize()
        );
        ImGui::SetCursorPos(cursorPos);
        //绘制小物件
        {
            CameraInfo cameraInfo = camera->GetCameraInfo();
            ImGuizmo::SetRect(viewPosition.x, viewPosition.y, viewSize.x, viewSize.y);
            ImGuizmo::SetOrthographic(false);
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
            {
                //选项
                const float width = ImGui::GetContentRegionAvail().x / 3 - 10;
                static constexpr char optionsName[3][10] = {"Position", "Rotation", "Scale"};
                static constexpr ImGuizmo::OPERATION options[] = {ImGuizmo::TRANSLATE, ImGuizmo::ROTATE, ImGuizmo::SCALE};
                static int selected = 0;
                for (int i = 0; i < 3; i++)
                {
                    ImGui::SetCursorPos(cursorPos + Vector2{(width + 10) * static_cast<float>(i), 0.0f});
                    if (ImGui::Selectable(optionsName[i], selected == i, 0, Vector2{width, 0.0f}))
                        selected = i;
                }
                if (ImGui::IsKeyDown(ImGuiKey_W))selected = 0;
                if (ImGui::IsKeyDown(ImGuiKey_E))selected = 1;
                if (ImGui::IsKeyDown(ImGuiKey_R))selected = 2;

                //绘制手柄
                if (target.IsNotNull())
                {
                    Matrix4x4 objectToWorld = target->GetLocalToWorldMatrix();
                    Manipulate(
                        reinterpret_cast<float*>(&cameraInfo.worldToView),
                        reinterpret_cast<float*>(&cameraInfo.viewToClip),
                        options[selected], ImGuizmo::LOCAL,
                        reinterpret_cast<float*>(&objectToWorld)
                    );
                    switch (selected)
                    {
                    case 0:
                        target->SetLocalPosition(static_cast<Vector3>(objectToWorld.GetColumn(3)));
                        break;
                    case 1:
                        {
                            //TODO 解码旋转
                        }
                        break;
                    case 2:
                        {
                            Matrix4x4 sqrScale = objectToWorld.GetTranspose() * objectToWorld;
                            target->SetLocalScale({sqrt(sqrScale.m00), sqrt(sqrScale.m11), sqrt(sqrScale.m22)});
                            break;
                        }
                    default: throw std::exception("数值越界");
                    }
                }
            }
        }
        //绘制帧率信息
        ImGui::Text(
            "Rate %.3f ms/frame (%.1f FPS)",
            static_cast<double>(Time::GetDeltaTime() * 1000.0f),
            static_cast<double>(1.0f / Time::GetDeltaTime())
        );
    }
}
