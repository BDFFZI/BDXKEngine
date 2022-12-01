#include "RenderEvent.h"

#include "BDXKEngine/Platform/GUI/GUI.h"
#include "Core/Camera.h"

namespace BDXKEngine
{
    ObjectPtr<Camera> RenderEvent::currentCamera = nullptr;

    const ObjectPtr<Camera>& RenderEvent::GetCurrentCamera()
    {
        return currentCamera;
    }

    void RenderEvent::Initialize(Window* window)
    {
        Texture::ResetDefaultRenderTarget();
        window->AddRenewEvent(Render);
        window->AddResizeEvent([](Vector2) { Texture::ResetDefaultRenderTarget(); });
    }
    void RenderEvent::Render()
    {
        //获取渲染事件
        const std::vector<ObjectPtr<Object>> drawGUIHandlers = ScriptableObject::FindScriptableObjectsOfType<DrawGUIHandler, Object>();
        const std::vector<ObjectPtr<Object>> drawGizmosHandlers = ScriptableObject::FindScriptableObjectsOfType<DrawGizmosHandler, Object>();
        //获取渲染资源
        const std::vector<Camera*>& cameraQueue = Camera::GetCameraQueue();
        const std::vector<Renderer*>& rendererQueue = Renderer::GetRendererQueue();
        const std::vector<Light*>& lightQueue = Light::GetLightQueue();
        for (const Light* light : lightQueue)light->UpdateShadowMap(rendererQueue);

        //相机渲染
        for (const auto& camera : cameraQueue)
        {
            ObjectPtr<Texture2D> rendererTarget = camera->GetRenderTarget();
            if (rendererTarget != nullptr)rendererTarget->SetRenderTarget();
            else Texture::SetRenderTargetDefault();

            camera->Render(lightQueue, rendererQueue);

            //绘制后期物体
            currentCamera = camera;
            for (const auto& drawGizmosHandler : drawGizmosHandlers)
                if (drawGizmosHandler.IsNotNull()) drawGizmosHandler.ToObject<DrawGizmosHandler>()->OnDrawGizmos();
        }

        //UI渲染
        {
            Texture::SetRenderTargetDefault();
            GUI::BeginDraw();

            for (const auto& drawGUIHandler : drawGUIHandlers)
                if (drawGUIHandler.IsNotNull()) drawGUIHandler.ToObject<DrawGUIHandler>()->OnDrawGUI();

            GUI::EndDraw();
        }

        GL::Present();
    }
}
