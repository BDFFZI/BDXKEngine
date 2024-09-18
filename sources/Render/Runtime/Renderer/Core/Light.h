﻿#pragma once
#include "Core/Runtime/Framework/Actor/Component.h"
#include "Render/Runtime/Graphics/GraphicsInfo.h"
#include "Core/Runtime/Platform/GL/Texture/Texture2D.h"
#include "Core/Runtime/Platform/GL/Texture/TextureCube.h"
#include "Renderer.h"

namespace BDXKEngine
{
    // 控制渲染开销，如精度更高的像素光照，或节省性能的顶点光照
    // enum class RenderMode
    // {
    //     Auto,
    //     Important,
    //     NotImportant,
    // };

    class Light : public Component
    {
    public:
        static const std::vector<Light*>& GetLightQueue(Vector3 position = Vector3::zero);

        LightInfo GetLightInfo(int order) const;
        ObjectPtr<Texture> GetShadowMap() const;
        LightType GetLightType() const;
        Color GetColor() const;
        float GetIntensity() const;

        void SetType(LightType type);
        void SetColor(Color color);
        void SetIntensity(float intensity);

        void UpdateShadowMap(const std::vector<Renderer*>& rendererQueue) const;
    private:
        static std::vector<Light*> lights;

        LightType lightType = LightType::Directional;
        Color color = Color::white;
        float intensity = 1;
        ObjectPtr<Texture2D> shadowMap;
        ObjectPtr<TextureCube> shadowMapCube;

        void Transfer(Transferer& transferer) override;
        void Awake() override;
        void Destroy() override;
        void Enable() override;
        void Disable() override;
    };

    CustomReflection(Light)
}
