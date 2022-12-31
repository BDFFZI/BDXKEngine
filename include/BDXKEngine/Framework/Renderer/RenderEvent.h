#pragma once
#include "BDXKEngine/Base/Object/Core/ObjectPtr.h"
#include "BDXKEngine/Platform/Window/Window.h"
#include "Core/Camera.h"

namespace BDXKEngine
{
    class RenderEvent;

    class DrawGUIHandler
    {
        friend RenderEvent;
    protected:
        virtual void OnDrawGUI() = 0;
        virtual ~DrawGUIHandler() = default;
    };

    class DrawGizmosHandler
    {
        friend RenderEvent;
    protected:
        virtual void OnDrawGizmos() = 0;
        virtual ~DrawGizmosHandler() = default;
    };

    class RenderEvent
    {
    public:
        static const ObjectPtr<Camera>& GetCurrentCamera();
        static const ObjectPtr<Texture2D>& GetCameraCanvas();
        static void Initialize(Window* window);
    private:
        static ObjectPtr<Camera> currentCamera;
        static ObjectPtr<Texture2D> cameraCanvas;
        static ObjectPtr<Material> blitMaterial;

        static void Render();
    };
}
