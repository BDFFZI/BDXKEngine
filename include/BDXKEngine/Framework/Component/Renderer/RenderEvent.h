#pragma once
#include "BDXKEngine/Base/Object/ObjectPtr.h"

namespace BDXKEngine
{
    class Camera;
    class RenderManager;

    class PreRenderFrameHandler
    {
        friend RenderManager;
    protected:
        virtual void OnPreRenderFrame() = 0;
        virtual ~PreRenderFrameHandler() = default;
    };

    class PostRenderFrameHandler
    {
        friend RenderManager;
    protected:
        virtual void OnPostRenderFrame() = 0;
        virtual ~PostRenderFrameHandler() = default;
    };

    class PreRenderCameraHandler
    {
        friend RenderManager;
    protected:
        virtual void OnPreRenderCamera(const ObjectPtr<Camera>& camera) = 0;
        virtual ~PreRenderCameraHandler() = default;
    };

    class PostRenderCameraHandler
    {
        friend RenderManager;
    protected:
        virtual void OnPostRenderCamera(const ObjectPtr<Camera>& camera) = 0;
        virtual ~PostRenderCameraHandler() = default;
    };

    class DrawGUIHandler
    {
        friend RenderManager;
    protected:
        virtual void OnDrawGUI() = 0;
        virtual ~DrawGUIHandler() = default;
    };

    class DrawGizmosHandler
    {
        friend RenderManager;
    protected:
        virtual void OnDrawGizmos() = 0;
        virtual ~DrawGizmosHandler() = default;
    };
}
