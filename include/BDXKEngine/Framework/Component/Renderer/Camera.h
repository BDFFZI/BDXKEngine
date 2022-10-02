#pragma once
#include "BDXKEngine/Function/Graphics.h"
#include "BDXKEngine/Framework/GameObject.h"
#include "BDXKEngine/Framework/Component/Component.h"
#include "BDXKEngine/Framework/Component/Transform.h"
#include "BDXKEngine/Framework/Component/Renderer/Light.h"

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

    class RenderManager;
    class Camera : public Component
    {
        friend RenderManager;
    public:
        float GetAspectRatio() const;
        ObjectPtr<Texture2D> GetRenderTarget();
        int GetDepth() const;

        void SetClearFlags(ClearFlags clearFlags);
        void SetBackground(Color color);
        void SetNearClipPlane(float distance);
        void SetFarClipPlane(float distance);
        void SetRenderTarget(const ObjectPtr<Texture2D>& renderTarget);
        void SetDepth(int depth);
    private:
        ObjectPtr<Texture2D> renderTarget = {};
        ClearFlags clearFlags = ClearFlags::Color;
        Projection projection = Projection::Perspective;
        Color background = Color::gray;
        float nearClipPlane = 0.3f;
        float farClipPlane = 1000;
        float fieldOfView = 60;
        float size = 5;
        int depth = 0;

        ObjectPtr<Transform> transform{};

        void Render(const std::vector<ObjectPtr<Renderer>>& renderers);

        void Transfer(Transferrer& transferrer) override;
        void Awake() override;
    };
}
