#pragma once
#include "BDXKEngine/Platform/Window/Window.h"

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
        static void Initialize(Window* window);
    private:
        static void Render();
    };
}
