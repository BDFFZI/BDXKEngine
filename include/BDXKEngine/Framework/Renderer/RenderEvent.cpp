#include "RenderEvent.h"
#include "BDXKEngine/Function/Graphics/Graphics.h"
#include "BDXKEngine/Platform/GUI/GUI.h"
#include "Core/Camera.h"
#include "Core/RenderSettings.h"

namespace BDXKEngine
{
    ObjectPtr<Camera> RenderEvent::currentCamera = nullptr;
    ObjectPtr<Texture2D> RenderEvent::cameraCanvas = nullptr;

    const ObjectPtr<Camera>& RenderEvent::GetCurrentCamera()
    {
        return currentCamera;
    }
    const ObjectPtr<Texture2D>& RenderEvent::GetCameraCanvas()
    {
        return cameraCanvas;
    }

    void RenderEvent::Initialize(Window* window)
    {
        Texture::ResetDefaultRenderTarget();
        const Vector2 ScreenSize = window->GetSize();
        cameraCanvas = Texture2D::Create(ScreenSize.GetXInt(), ScreenSize.GetYInt(), TextureFormat::B8G8R8A8_UNORM);

        window->AddRenewEvent(Render);
        window->AddResizeEvent([](Vector2 size)
        {
            if (size.x <= 0 || size.y <= 0)
                return;
            
            Texture::ResetDefaultRenderTarget();
            cameraCanvas = Texture2D::Create(size.GetXInt(), size.GetYInt(), TextureFormat::B8G8R8A8_UNORM);
        });
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
            ObjectPtr<Texture2D> renderTarget = camera->GetRenderTarget();
            if (renderTarget != nullptr)renderTarget->SetRenderTarget();
            else cameraCanvas->SetRenderTarget();

            camera->Render(lightQueue, rendererQueue);

            //绘制后期物体
            currentCamera = camera;
            for (const auto& drawGizmosHandler : drawGizmosHandlers)
                if (drawGizmosHandler.IsNotNull()) drawGizmosHandler.ToObject<DrawGizmosHandler>()->OnDrawGizmos();
        }

        //将相机渲染传输到屏幕
        Texture::SetRenderTargetDefault();
        GL::Clear(true, true);
        const ObjectPtr<Material> material = RenderSettings::GetUnlitMaterial();
        material->SetMatrix(0, Matrix4x4::identity);
        material->SetTexture2D(0, cameraCanvas);
        material->UploadRP(PassType::ForwardBase);
        Graphics::DrawViewport();

        //UI渲染
        {
            GUI::BeginDraw();

            for (const auto& drawGUIHandler : drawGUIHandlers)
                if (drawGUIHandler.IsNotNull()) drawGUIHandler.ToObject<DrawGUIHandler>()->OnDrawGUI();

            GUI::EndDraw();
        }


        GL::Present();
    }
}
