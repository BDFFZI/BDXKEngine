#include "RenderEvent.h"
#include "BDXKEngine/Function/Graphics/Graphics.h"
#include "BDXKEngine/Function/Resources/ResourcesDefault.h"
#include "BDXKEngine/Platform/GUI/GUI.h"
#include "BDXKEngine/Platform/Serialization/Serialization.h"
#include "Core/Camera.h"

namespace BDXKEngine
{
    ObjectPtr<Camera> RenderEvent::currentCamera = nullptr;
    ObjectPtr<Texture2D> RenderEvent::cameraCanvas = nullptr;
    ObjectPtr<Material> RenderEvent::blitMaterial = nullptr;

    const ObjectPtr<Camera>& RenderEvent::GetCurrentCamera()
    {
        return currentCamera;
    }
    const ObjectPtr<Texture2D>& RenderEvent::GetCameraCanvas()
    {
        return cameraCanvas;
    }

    void RenderEvent::Initialize()
    {
        Texture::ResetDefaultRenderTarget();
        const Vector2 ScreenSize = Window::GetSize();
        cameraCanvas = Texture2D::Create(ScreenSize.GetXInt(), ScreenSize.GetYInt(), TextureFormat::R8G8B8A8_UNORM);
        blitMaterial = Serialization::Clone(ResourcesDefault::GetUnlitMaterial());
        blitMaterial->SetMatrix(0, Matrix4x4::identity);
        blitMaterial->SetTexture2D(0, cameraCanvas);

        Window::AddRenewEvent(Render);
        Window::AddResizeEvent([](Vector2 size)
        {
            if (size.x <= 0 || size.y <= 0)
                return;

            Texture::ResetDefaultRenderTarget();
            cameraCanvas = Texture2D::Create(size.GetXInt(), size.GetYInt(), TextureFormat::R8G8B8A8_UNORM);
            blitMaterial->SetTexture2D(0, cameraCanvas);
        });
    }
    void RenderEvent::Render()
    {
        //获取渲染事件
        const std::vector<ObjectPtr<ScriptableObject>> drawGUIHandlers
            = ScriptableObject::FindScriptableObjectsOfType<DrawGUIHandler, ScriptableObject>();
        const std::vector<ObjectPtr<ScriptableObject>> drawGizmosHandlers
            = ScriptableObject::FindScriptableObjectsOfType<DrawGizmosHandler, ScriptableObject>();
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
        if (blitMaterial->UploadRP(PassType::ForwardBase))
            Graphics::DrawViewport();

        //UI渲染
        {
            GUI::BeginDraw();

            for (const auto& drawGUIHandler : drawGUIHandlers)
                if (drawGUIHandler.IsNotNull() && drawGUIHandler->IsNotResource()) drawGUIHandler.ToObject<DrawGUIHandler>()->OnDrawGUI();

            GUI::EndDraw();
        }

        GL::Present();
    }
}
