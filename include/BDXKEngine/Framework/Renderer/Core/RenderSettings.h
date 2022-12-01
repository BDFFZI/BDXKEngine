#pragma once
#include "BDXKEngine/Framework/Core/Singleton.h"
#include "BDXKEngine/Function/Graphics/Material.h"
#include "BDXKEngine/Platform/GL/Texture/TextureCube.h"

namespace BDXKEngine
{
    class RenderSettings : public Singleton<RenderSettings>
    {
    public:
        static void Reset();

        const ObjectPtr<TextureCube>& GetSkybox() const;
        const ObjectPtr<Material>& GetSkyboxMaterial() const;
        void SetSkybox(const ObjectPtr<TextureCube>& skybox);
        void SetSkyboxMaterial(const ObjectPtr<Material>& skyboxMaterial);
    private:
        ObjectPtr<TextureCube> skybox = nullptr;
        ObjectPtr<Material> skyboxMaterial = nullptr;
        void Transfer(Transferer& transferer) override;
    };

    CustomReflection(RenderSettings)
}
