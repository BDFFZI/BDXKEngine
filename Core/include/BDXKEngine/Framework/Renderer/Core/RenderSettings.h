#pragma once
#include "BDXKEngine/Framework/Core/Singleton.h"
#include "BDXKEngine/Function/Graphics/Material.h"
#include "BDXKEngine/Platform/GL/Texture/TextureCube.h"

namespace BDXKEngine
{
    class RenderSettings : public Singleton<RenderSettings>
    {
    public:
        static const ObjectPtr<TextureCube>& GetSkybox();
        static const ObjectPtr<Material>& GetSkyboxMaterial();
        static const ObjectPtr<Material>& GetUnlitMaterial();
        static void SetSkybox(const ObjectPtr<TextureCube>& skybox);
        static void SetSkyboxMaterial(const ObjectPtr<Material>& skyboxMaterial);
        static void SetUnlitMaterial(const ObjectPtr<Material>& unlitMaterial);
    private:
        ObjectPtr<TextureCube> skybox = nullptr;
        ObjectPtr<Material> skyboxMaterial = nullptr;
        ObjectPtr<Material> unlitMaterial = nullptr;

        void Transfer(Transferer& transferer) override;
    };

    CustomReflection(RenderSettings)
}
