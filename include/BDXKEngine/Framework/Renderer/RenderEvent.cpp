#include "RenderEvent.h"

#include "BDXKEngine/Platform/GUI/GUI.h"
#include "Core/Camera.h"

namespace BDXKEngine
{
    void RenderEvent::Initialize(Window* window)
    {
        Texture::ResetDefaultRenderTarget();
        window->AddRenewEvent(Render);
        window->AddResizeEvent([](Vector2) { Texture::ResetDefaultRenderTarget(); });
    }
    void RenderEvent::Render()
    {
        //获取渲染事件
        const std::vector<DrawGUIHandler*> drawGUIHandlers = ScriptableObject::FindScriptableObjectPtrsOfType<DrawGUIHandler>();
        const std::vector<DrawGizmosHandler*> drawGizmosHandlers = ScriptableObject::FindScriptableObjectPtrsOfType<DrawGizmosHandler>();
        //获取渲染资源
        const std::vector<Camera*>& cameraQueue = Camera::GetCameraQueue();
        const std::vector<Renderer*>& rendererQueue = Renderer::GetRendererQueue();
        const std::vector<Light*>& lightQueue = Light::GetLightQueue();
        for (const Light* light : lightQueue)light->UpdateShadowMap(rendererQueue);

        Texture::SetRenderTargetDefault();
        GL::Clear(true, true, Color::clear);

        //标准渲染管线（相机渲染）
        for (const auto& camera : cameraQueue)
        {
            ObjectPtr<Texture2D> rendererTarget = camera->GetRenderTarget();
            if (rendererTarget != nullptr)rendererTarget->SetRenderTarget();
            else Texture::SetRenderTargetDefault();

            camera->Render(lightQueue, rendererQueue);
        }

        //UI渲染
        {
            Texture::SetRenderTargetDefault();
            GUI::BeginDraw();
        
            //绘制后期物体
            for (const auto drawGizmosHandler : drawGizmosHandlers)
                drawGizmosHandler->OnDrawGizmos();
        
            //绘制UI
            for (const auto drawGUIHandler : drawGUIHandlers)
                drawGUIHandler->OnDrawGUI();
        
            GUI::EndDraw();
        }
        
        GL::Present();
    }
}
