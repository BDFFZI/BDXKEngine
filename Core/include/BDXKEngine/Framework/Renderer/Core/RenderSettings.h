#pragma once
#include "BDXKEngine/Framework/Core/Singleton.h"
#include "BDXKEngine/Function/Graphics/Material.h"

namespace BDXKEngine
{
    class RenderSettings : public Singleton<RenderSettings>
    {
    public:
        static const ObjectPtr<Material>& GetSkybox();
        static const ObjectPtr<Material>& GetBlitMaterial();
        static void SetSkybox(const ObjectPtr<Material>& skybox);
        static void SetBlitMaterial(const ObjectPtr<Material>& blitMaterial);
    private:
        ObjectPtr<Material> skybox = nullptr;
        ObjectPtr<Material> blitMaterial = nullptr;
        void Transfer(Transferer& transferer) override;
    };

    CustomReflection(RenderSettings)
}
