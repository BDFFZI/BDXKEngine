#pragma once
#include "RenderEvent.h"
#include "BDXKEngine/Base/Color.h"
#include "BDXKEngine/Platform/GL/Resources/Texture2D.h"
#include "BDXKEngine/Platform/GL/Resources/TextureCube.h"
#include "BDXKEngine/Function/Graphics.h"
#include "BDXKEngine/Framework/Component/Renderer/Renderer.h"

namespace BDXKEngine
{
    class LightEditor;

    class Light : public Component, public PreRenderFrameHandler
    {
        friend LightEditor;

    public:
        LightType GetType() const;
        Color GetColor() const;
        float GetIntensity() const;
        RenderMode GetRenderMode() const;

        void SetLightType(LightType type);
        void SetColor(Color color);
        void SetIntensity(float intensity);
    private:
        static std::vector<ObjectPtr<Light>> lights;

        LightType lightType = LightType::Directional;
        RenderMode renderMode = RenderMode::Important;
        Color color = Color::white;
        float intensity = 1;
        ObjectPtr<Texture2D> shadowMap;
        ObjectPtr<TextureCube> shadowMapCube;

        LightInfo GetLightInfo(int order = 0);
        ShadowInfo GetShadowInfo();
        ObjectPtr<Texture> GetShadowMap();

        void OnPreRenderFrame() override;

        void Enable() override;
        void Disable() override;
        void Transfer(transferer& transferer) override;
        void PreAwake() override;
        void PreDestroy() override;
    };

    class LightEditor
    {
    public:
        static LightInfo GetLightInfo(const ObjectPtr<Light>& light, int order = 0);
        static ShadowInfo GetShadowInfo(const ObjectPtr<Light>& light);
        static ObjectPtr<Texture> GetShadowMap(const ObjectPtr<Light>& light);
        static std::vector<ObjectPtr<Light>>& GetLights();
    };
}
