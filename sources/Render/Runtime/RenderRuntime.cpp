#include "RenderRuntime.h"
#include "Core/Runtime/Function/GUI/GUI.h"
#include "Core/Runtime/Base/Object/Serialization/ObjectSerialization.h"
#include "Core/Runtime/Platform/Window/Screen.h"
#include "Core/Runtime/Platform/GL/Shader/Shader.h"
#include "Graphics/Graphics.h"
#include "Renderer/MeshRenderer.h"

namespace BDXKEngine
{
    const ObjectPtr<Camera>& RenderRuntime::GetCurrentCamera()
    {
        return currentCamera;
    }
    const ObjectPtr<Texture2D>& RenderRuntime::GetCameraCanvas()
    {
        return cameraCanvas;
    }

    void RenderRuntime::Resize(const Vector2& size)
    {
        if (size.x <= 0 || size.y <= 0)
            return;

        Texture::ResetDefaultRenderTarget();
        cameraCanvas = Texture2D::Create(size.GetXInt(), size.GetYInt(), TextureFormat::R8G8B8A8_UNORM);
    }

    void RenderRuntime::OnEngineStart()
    {
        Graphics::Initialize();

        blitShader = Shader::Create(R"(
        Texture2D texture2D0:register(t0);
        SamplerState samplerState0:register(s0);
        struct Pixel
        {
            float4 positionCS:SV_POSITION;
            float2 uv:TEXCOORD;
        };
        
        Pixel VertexPass(float3 position:POSITION,float2 uv:TEXCOORD)
        {
            Pixel pixel;
            pixel.positionCS = float4(position,1);
            pixel.uv = uv;
            return pixel;
        }

        float4 PixelPass(Pixel pixel):SV_Target
        {
            return texture2D0.Sample(samplerState0,pixel.uv);
        }
            )");

        Resize(Screen::GetSize());
    }
    void RenderRuntime::OnEngineUpdate()
    {
        if (const Vector2 ScreenSize = Screen::GetSize(); cameraCanvas->GetSize() != ScreenSize)
            Resize(ScreenSize);

        //获取渲染事件
        const std::vector<DrawGizmosHandler*> drawGizmosHandlers = Component::FindComponentsOfType<DrawGizmosHandler, DrawGizmosHandler*>();
        //获取渲染资源
        const std::vector<Camera*>& cameraQueue = Camera::GetCameraQueue();
        const std::vector<Renderer*>& rendererQueue = Renderer::GetRendererQueue();
        const std::vector<Light*>& lightQueue = Light::GetLightQueue();
        for (const Light* light : lightQueue)
            light->UpdateShadowMap(rendererQueue);

        //相机渲染
        for (const auto& camera : cameraQueue)
        {
            if (camera->GetGameObject()->GetScene().IsNull())
                continue;

            ObjectPtr<Texture2D> renderTarget = camera->GetRenderTarget();
            if (renderTarget != nullptr)renderTarget->SetRenderTarget();
            else cameraCanvas->SetRenderTarget();

            camera->Render(lightQueue, rendererQueue);

            //绘制后期物体
            currentCamera = camera;
            for (const auto& drawGizmosHandler : drawGizmosHandlers)
                drawGizmosHandler->OnDrawGizmos();
        }

        //将相机渲染传输到屏幕
        Texture::SetRenderTargetDefault();
        GL::Clear(true, true);
        blitShader->UploadRP();
        cameraCanvas->UploadRP(0);
        Graphics::DrawViewport(false);
    }
}
