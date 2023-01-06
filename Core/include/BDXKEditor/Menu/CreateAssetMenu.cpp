#include "CreateAssetMenu.h"
#include "BDXKEditor/Function/AssetsBuiltIn.h"
#include "BDXKEngine/Platform/Serialization/Serialization.h"

namespace BDXKEditor
{
    CustomCreateAssetMenuRegister::CustomCreateAssetMenuRegister(
        const char* funcName, const std::function<ObjectPtrBase()>& func, const char* assetName
    )
    {
        CreateAssetMenu::AddMenuItem(funcName, func, assetName);
    }

    ObjectPtr<Material> CreateAssetMenu::CreateStandardMaterial()
    {
        const auto standardMaterial = Material::Create({
            {PassType::ForwardBase, AssetsBuiltIn::GetStandardShader()},
            {PassType::ForwardAdd, AssetsBuiltIn::GetStandardShader()},
            {PassType::ShadowCaster, AssetsBuiltIn::GetShadowMapShader()}
        });
        standardMaterial->SetPassBlend(1, Blend::Additive);
        return standardMaterial;
    }
    ObjectPtr<Material> CreateAssetMenu::CreateUnlitMaterial()
    {
        const auto unlitMaterial = Material::Create({
            {PassType::ForwardBase, AssetsBuiltIn::GetUnlitShader()},
            {PassType::ForwardAdd, AssetsBuiltIn::GetUnlitShader()},
            {PassType::ShadowCaster, AssetsBuiltIn::GetShadowMapShader()}
        });
        unlitMaterial->SetPassBlend(1, Blend::Additive);
        return unlitMaterial;
    }
    ObjectPtr<Material> CreateAssetMenu::CreateSkyboxMaterial()
    {
        const auto skyboxMaterial = Material::Create({
            {PassType::ForwardBase, AssetsBuiltIn::GetSkyboxShader()}
        });
        skyboxMaterial->SetPassZTest(0, ZTest::Background);
        return skyboxMaterial;
    }
    ObjectPtr<PhysicMaterial> CreateAssetMenu::CreatePhysicMaterial()
    {
        return Object::Create<PhysicMaterial>();
    }
}
