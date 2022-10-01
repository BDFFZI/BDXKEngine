#include "SceneView.h"
#include "BDXKEditor/Menu/CreationMenu.h"
#include "BDXKEditor/Tools/CameraController.h"

namespace BDXKEditor
{
    void SceneView::OnPreRenderFrame()
    {
        if (editorCameraTexture->GetSize() != viewSize && viewSize.x * viewSize.y > 0)
        {
            editorCameraTexture = Texture2D::Create(viewSize.GetXInt(), viewSize.GetYInt());
            editorCamera->SetRenderTarget(editorCameraTexture);
        }
    }
    void SceneView::OnDrawWindow()
    {
        ImGui::Image(
            IMGUIManager::GetImTextureID(editorCameraTexture),
            editorCameraTexture->GetSize()
        );
        ImGui::SameLine(-1);
        ImGui::Text(" 平均帧率 %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

        viewSize = Vector2{ImGui::GetWindowSize()} - Vector2{0, 40};

        // {
        //     static bool show_demo_window = true;
        //     static bool show_another_window = false;
        //     static float f = 0.0f;
        //     static int counter = 0;
        //
        //     ImGui::Begin("Hello, world!"); // Create a window called "Hello, world!" and append into it.
        //
        //     ImGui::Text("This is some useful text."); // Display some text (you can use a format strings too)
        //     ImGui::Checkbox("Demo Window", &show_demo_window); // Edit bools storing our window open/close state
        //     ImGui::Checkbox("Another Window", &show_another_window);
        //
        //     ImGui::SliderFloat("float", &f, 0.0f, 1.0f); // Edit 1 float using a slider from 0.0f to 1.0f
        //
        //     if (ImGui::Button("Button")) // Buttons return true when clicked (most widgets return true when edited/activated)
        //         counter++;
        //     ImGui::SameLine();
        //     ImGui::Text("counter = %d", counter);
        //

        //     ImGui::End();
        // }


        //显示所有游戏物体
        // float drawY = 10;
        // std::vector<ObjectPtr<Transform>> rootTransforms = TransformEditor::GetRootTransforms();
        // for (ObjectPtr<Transform> transform : rootTransforms)
        // {
        //     drawY = ShowGameObjectInfo(transform->GetGameObject(), drawY);
        // }
        //
        // //显示描述
        // const Vector2 screenSize = Screen::GetSize();
        // Rect rect = {};
        // rect.SetSize(Vector2(screenSize.x / 4, screenSize.y / 2));
        // rect.SetPosition(Vector2{screenSize.x - rect.width - 10, 10.0f});
        // if (focus.IsNotNull())GUI::TextArea(rect, focus->ToString());
        // else GUI::TextArea(rect, "");
        //
        // rect.y += rect.height + 10;
        // rect.height = 25;
        //
        // //显示帧率
        // GUI::TextArea(rect, "帧率:" + std::to_string(frameRate = (int)std::lerp(frameRate, 1 / Time::GetDeltaTime(), 0.1f)));
        // rect.y += rect.height + 10;
        //
        // //孤儿箱:用来将节点父亲设为空
        // GUI::TextArea(rect, "孤儿箱");
        // ObjectPtr<Transform> transform = nullptr;
        // if (Event::IsDrop(rect, reinterpret_cast<ObjectPtr<Component>*>(&transform)))
        // {
        //     if (transform != nullptr)
        //     {
        //         transform->SetParent(nullptr);
        //     }
        // }
        // rect.y += rect.height + 10;
        //
        // if (focus.IsNotNull() && GUI::Button(rect, "切换启用状态"))
        // {
        //     focus->SetIsEnabling(!focus->GetIsEnabling());
        // }
        // rect.y += rect.height + 10;
    }
    void SceneView::OnShow()
    {
        const ObjectPtr<GameObject> editorCameraGameObject = CreationMenu::Camera("EditorCamera");
        editorCameraGameObject->AddComponent<CameraController>();

        editorCamera = editorCameraGameObject->GetComponent<Camera>();
        editorCameraTexture = Texture2D::Create(960, 540);
        viewSize = {960, 540};

        editorCamera->SetRenderTarget(editorCameraTexture);
    }
}
