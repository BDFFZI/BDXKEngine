#pragma once
#include "Render/Runtime/Graphics/Material.h"

namespace BDXKEngine
{
    class RenderSetting : public Object
    {
    public:
        static const ObjectPtr<Material>& GetSkybox();
        static void SetSkybox(const ObjectPtr<Material>& skybox);

    private:
        inline static ObjectPtr<Material> skybox = nullptr;

        void Transfer(Transferer& transferer) override;
    };

    CustomReflection(RenderSetting)
}
