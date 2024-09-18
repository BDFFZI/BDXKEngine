#include "RenderAssetsMenu.h"
#include <Render/Editor/AssetImporter/RenderAssets.h>

namespace BDXKEditor
{
    ObjectPtr<Material> RenderAssetsMenu::CreateStandardMaterial()
    {
        const auto standardMaterial = Material::Create({
            {PassType::ForwardBase, RenderAssets::GetStandardShader()},
            {PassType::ForwardAdd, RenderAssets::GetStandardShader()},
            {PassType::ShadowCaster, RenderAssets::GetShadowMapShader()}
        });
        standardMaterial->SetPassBlend(1, Blend::Additive);
        return standardMaterial;
    }
    ObjectPtr<Material> RenderAssetsMenu::CreateUnlitMaterial()
    {
        const auto unlitMaterial = Material::Create({
            {PassType::ForwardBase, RenderAssets::GetUnlitShader()},
            {PassType::ForwardAdd, RenderAssets::GetUnlitShader()},
            {PassType::ShadowCaster, RenderAssets::GetShadowMapShader()}
        });
        unlitMaterial->SetPassBlend(1, Blend::Additive);
        return unlitMaterial;
    }
    ObjectPtr<Material> RenderAssetsMenu::CreateSkyboxMaterial()
    {
        const auto skyboxMaterial = Material::Create({
            {PassType::ForwardBase, RenderAssets::GetSkyboxShader()}
        });
        skyboxMaterial->SetPassZTest(0, ZTest::Background);
        return skyboxMaterial;
    }
}
