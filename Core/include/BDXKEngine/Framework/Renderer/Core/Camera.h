#pragma once
#include "Light.h"
#include "Renderer.h"
#include "BDXKEngine/Framework/Core/Component.h"
#include "BDXKEngine/Function/Graphics/GraphicsInfo.h"
#include "BDXKEngine/Platform/GL/Texture/Texture2D.h"

namespace BDXKEngine
{
    enum class ClearFlags
    {
        Not,
        Color,
        Skybox,
    };

    enum class Projection
    {
        Orthographic,
        Perspective
    };
    
    class Camera : public Component
    {
    public:
        static const std::vector<Camera*>& GetCameraQueue();
        
        CameraInfo GetCameraInfo() const;
        ObjectPtr<Texture2D> GetRenderTarget();
        ClearFlags GetClearFlags() const;
        Projection GetProjection() const;
        const Color& GetBackground() const;
        int GetPriority() const;
        float GetAspectRatio() const;
        
        void SetRenderTarget(const ObjectPtr<Texture2D>& renderTarget);
        void SetClearFlags(ClearFlags clearFlags);
        void SetProjection(Projection projection);
        void SetBackground(Color color);
        void SetPriority(int depth);
        void SetNearClipPlane(float distance);
        void SetFarClipPlane(float distance);

        void Render(const std::vector<Light*>& lightQueue, const std::vector<Renderer*>& rendererQueue) const;
    private:
        static std::vector<Camera*> cameras;
        
        ObjectPtr<Texture2D> renderTarget = {};
        ClearFlags clearFlags = ClearFlags::Color;
        Projection projection = Projection::Perspective;
        Color background = Color::gray;
        float nearClipPlane = 0.3f;
        float farClipPlane = 1000;
        float fieldOfView = 60;
        float size = 5;
        int priority = 0;

        void Transfer(Transferer& transferer) override;
        void Enable() override;
        void Disable() override;
    };

    CustomReflection(Camera)
}
