#include "RenderManager.h"
#include <algorithm>
#include "BDXKEngine/Function/GUI.h"
#include "BDXKEngine/Framework/Component/Renderer/Renderer.h"

namespace BDXKEngine
{
    std::vector<ObjectPtr<Renderer>> RenderManager::renderers = {};

    RenderManager* RenderManager::Initialize(Window* window)
    {
        window->AddRenewEvent([]
        {
            //获取渲染事件
            const std::vector<PreRenderFrameHandler*> preRenderFrameHandlers =
                ScriptableObject::FindScriptableObjectsOfType<PreRenderFrameHandler>();
            const std::vector<PostRenderFrameHandler*> postRenderFrameHandlers =
                ScriptableObject::FindScriptableObjectsOfType<PostRenderFrameHandler>();
            const std::vector<PreRenderCameraHandler*> preRenderHandlers =
                ScriptableObject::FindScriptableObjectsOfType<PreRenderCameraHandler>();
            const std::vector<PostRenderCameraHandler*> postRenderHandlers =
                ScriptableObject::FindScriptableObjectsOfType<PostRenderCameraHandler>();
            const std::vector<DrawGUIHandler*> drawGUIHandlers = ScriptableObject::FindScriptableObjectsOfType<DrawGUIHandler>();
            const std::vector<DrawGizmosHandler*> drawGizmosHandlers = ScriptableObject::FindScriptableObjectsOfType<DrawGizmosHandler>();

            for (const auto preRenderFrameHandler : preRenderFrameHandlers)
                preRenderFrameHandler->OnPreRenderFrame();

            GL2D::BeginDraw();
            IMGUIManager::BeginDraw();

            //标准渲染管线（相机渲染）
            const std::vector<ObjectPtr<Camera>> cameras = GetCamerasQueue();
            const std::vector<ObjectPtr<Renderer>> renderers = GetRenderersQueue();
            for (const auto& camera : cameras)
            {
                SetRenderTarget(camera->GetRenderTarget());

                //如：渲染阴影贴图
                for (const auto preRenderHandler : preRenderHandlers)
                    preRenderHandler->OnPreRenderCamera(camera);

                camera->Render(renderers);

                for (const auto postRenderHandler : postRenderHandlers)
                    postRenderHandler->OnPostRenderCamera(camera);
            }

            //UI渲染
            {
                SetRenderTarget(nullptr);

                //绘制后期物体
                for (const auto drawGizmosHandler : drawGizmosHandlers)
                    drawGizmosHandler->OnDrawGizmos();

                //绘制UI
                for (const auto drawGUIHandler : drawGUIHandlers)
                    drawGUIHandler->OnDrawGUI();
            }

            IMGUIManager::EndDraw();
            GL2D::EndDraw();

            for (const auto postRenderFrameHandler : postRenderFrameHandlers)
                postRenderFrameHandler->OnPostRenderFrame();

            Present(); //将默认渲染目标输出到屏幕
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

    std::vector<ObjectPtr<Renderer>> RenderManager::GetRenderersQueue()
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
    std::vector<ObjectPtr<Camera>> RenderManager::GetCamerasQueue()
    {
        auto cameras = ScriptableObject::FindScriptableObjectsOfType<Camera>();
        std::ranges::sort(cameras, [](const Camera* a, const Camera* b) { return a->depth < b->depth; });
        return {cameras.begin(), cameras.end()};
    }
}
