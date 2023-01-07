#pragma once
#include "BDXKEngine/Framework/Core/Singleton.h"
#include "BDXKEngine/Function/Graphics/Material.h"

namespace BDXKEngine
{
    class RenderSettings : public Singleton<RenderSettings>
    {
    public:
        static const ObjectPtr<Material>& GetSkybox();
        static void SetSkybox(const ObjectPtr<Material>& skybox);
    private:
        ObjectPtr<Material> skybox = nullptr;
        void Transfer(Transferer& transferer) override;
    };

    CustomReflection(RenderSettings)
}
