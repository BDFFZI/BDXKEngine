#pragma once
#include "Core/Editor/EditorMenu/AssetMenu.h"
#include "Render/Runtime/Graphics/Material.h"

namespace BDXKEditor
{
    using namespace BDXKEngine;

    class RenderAssetsMenu
    {
    public:
        static ObjectPtr<Material> CreateStandardMaterial();
        static ObjectPtr<Material> CreateUnlitMaterial();
        static ObjectPtr<Material> CreateSkyboxMaterial();

    private:
        inline static std::unordered_map<std::string, std::string> assetNames = {};
        CustomCreateAssetMenu("Render/StandardMaterial", CreateStandardMaterial, "Standard.material")
        CustomCreateAssetMenu("Render/UnlitMaterial", CreateUnlitMaterial, "Unlit.material")
        CustomCreateAssetMenu("Render/SkyboxMaterial", CreateSkyboxMaterial, "Skybox.material")
    };
}
