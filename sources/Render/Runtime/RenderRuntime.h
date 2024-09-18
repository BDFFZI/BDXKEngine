#pragma once
#include "Core/Runtime/CoreRuntime.h"
#include "Core/Runtime/Platform/GL/Shader/Shader.h"
#include "Renderer/Core/Camera.h"

namespace BDXKEngine
{
    class RenderRuntime;

    class DrawGizmosHandler
    {
        friend RenderRuntime;

    protected:
        virtual void OnDrawGizmos() = 0;
        virtual ~DrawGizmosHandler() = default;
    };

    class RenderRuntime
    {
    public:
        static const ObjectPtr<Camera>& GetCurrentCamera();
        static const ObjectPtr<Texture2D>& GetCameraCanvas();

    private:
        inline static ObjectPtr<Texture2D> cameraCanvas; //默认RenderTarget
        inline static ObjectPtr<Shader> blitShader;
        inline static ObjectPtr<Camera> currentCamera;

        static void Resize(const Vector2& size);

        static void OnEngineStart();
        static void OnEngineUpdate();
        CustomEngineStartEvent(OnEngineStart, 0)
        CustomEngineUpdateEvent(OnEngineUpdate, 20)
    };
}
