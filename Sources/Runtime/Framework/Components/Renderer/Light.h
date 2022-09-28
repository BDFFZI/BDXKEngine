#pragma once
#include "Base/Color.h"
#include "Platform/GL/Resources/Texture2D.h"
#include "Platform/GL/Resources/TextureCube.h"
#include "Function/Graphics.h"
#include "Framework/Components/Renderer/Renderer.h"
#include "Framework/Components/Renderer/RendererManager.h"

namespace BDXKEngine
{
    class LightEditor;

    class Light : public Component, public PreRenderHandler, RendererManager
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

        void OnPreRender() override;

        void Enable() override;
        void Disable() override;
        
        void Transfer(Transferrer& transferrer) override;
        void Awake() override;
        void Destroy() override;
    };

    class LightEditor
    {
    protected:
        static LightInfo GetLightInfo(const ObjectPtr<Light>& light, int order = 0);
        static ShadowInfo GetShadowInfo(const ObjectPtr<Light>& light);
        static ObjectPtr<Texture> GetShadowMap(const ObjectPtr<Light>& light);
        static std::vector<ObjectPtr<Light>>& GetLights();
    };
}
