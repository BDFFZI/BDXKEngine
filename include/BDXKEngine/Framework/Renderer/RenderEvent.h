#pragma once
#include "BDXKEngine/Base/Object/ObjectPtr.h"
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
        static void Initialize(Window* window);
    private:
        static ObjectPtr<Camera> currentCamera;

        static void Render();
    };
}
