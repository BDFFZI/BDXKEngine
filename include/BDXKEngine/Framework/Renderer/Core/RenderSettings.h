#pragma once
#include "BDXKEngine/Framework/Core/Singleton.h"
#include "BDXKEngine/Function/Graphics/Material.h"
#include "BDXKEngine/Platform/GL/Texture/TextureCube.h"

namespace BDXKEngine
{
    struct RenderSettings : Singleton<RenderSettings>
    {
        static ObjectPtr<RenderSettings> Create();
        
        ObjectPtr<TextureCube> skybox = nullptr;
        ObjectPtr<Material> skyboxMaterial;

        void Transfer(Transferer& transferer) override;
    };
}
