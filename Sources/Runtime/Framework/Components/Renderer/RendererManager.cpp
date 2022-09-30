#include "RendererManager.h"
#include <algorithm>
#include "Function/GUI.h"
#include "Framework/Components/Renderer/Camera.h"
#include "Framework/Components/Renderer/Renderer.h"

namespace BDXKEngine
{
    std::vector<ObjectPtr<Renderer>> RendererManager::renderers = {};
    ObjectPtr<Texture2D> RendererManager::renderTexture = {};

    RendererManager* RendererManager::Initialize(Window* window)
    {
        const Vector2 size = window->GetSize();
        renderTexture = Texture2D::Create(static_cast<int>(size.x), static_cast<int>(size.y));

        window->AddRenewEvent([]
        {
            //获取渲染事件
            const std::vector<PreRenderHandler*> preRenderHandlers = Object::FindObjectsOfType<PreRenderHandler>();
            const std::vector<PostRenderHandler*> postRenderHandlers = Object::FindObjectsOfType<PostRenderHandler>();
            const std::vector<Camera*> cameras = Object::FindObjectsOfType<Camera>();

            GL::SetRenderTarget(nullptr);
            GL2D::BeginDraw();
            IMGUIManager::BeginDraw();

            Clear(true, true);

            for (const auto camera : cameras)
            {
                //如：渲染阴影贴图
                for (const auto preRenderHandler : preRenderHandlers)
                    if (dynamic_cast<Component*>(preRenderHandler)->IsActivatingAndEnabling()) preRenderHandler->OnPreRender();

                //标准渲染管线（相机渲染）
                SetRenderTarget(renderTexture);
                camera->Render();
                SetRenderTarget(nullptr);


                ImGui::Begin("GameView", nullptr);

                ImGui::Image(
                    TextureEditor::GetShaderResourceView(renderTexture),
                    Vector2(static_cast<float>(renderTexture->GetWidth()), static_cast<float>(renderTexture->GetHeight()))
                );

                //如：绘制UI，后期物体等
                for (const auto postRenderHandler : postRenderHandlers)
                    if (dynamic_cast<Component*>(postRenderHandler)->IsActivatingAndEnabling()) postRenderHandler->OnPostRender();

                ImGui::End();
            }

            IMGUIManager::EndDraw();
            GL2D::EndDraw(); //顺序很重要，Direct2D的渲染结果最终需要Direct3D来显示
            GL::Present(); //显示到屏幕上
        });
        window->AddResizeEvent([](Vector2 size)
        {
            GL2D::ReleaseResources(); //释放对Direct3D渲染纹理的引用，从而使Direct3D可以重新创建纹理

            //重新调整纹理大小
            Rect rect = {Vector2::zero, size};
            GL::ResizeDefaultRenderTarget(rect.GetSize());

            GL2D::CreateResources(); //使用Direct3D新创建的纹理
        });
        window->AddDestroyEvent([]
        {
            renderTexture = nullptr;
        });
        return nullptr;
    }

    std::vector<ObjectPtr<Renderer>> RendererManager::GetRenderersQueue()
    {
        std::vector<ObjectPtr<Renderer>> renderersQueue = renderers;
        for (const auto& renderer : renderersQueue)
            if (renderer->GetMaterial().IsNull() || renderer->GetMesh().IsNull())
                throw std::exception("渲染器的材质球或网格为空");

        std::ranges::sort(
            renderersQueue,
            [](const ObjectPtr<Renderer>& a, const ObjectPtr<Renderer>& b)
            {
                return a->GetMaterial()->GetRenderQueue() < b->GetMaterial()->GetRenderQueue();
            }
        );
        return renderersQueue;
    }
}
