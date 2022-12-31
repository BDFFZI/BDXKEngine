#include "ResourcesDefault.h"

#include "BDXKEngine/Platform/GL/Mesh/MeshImport.h"
#include "BDXKEngine/Platform/GL/Shader/ShaderImport.h"

namespace BDXKEngine
{
    ObjectPtr<Material> ResourcesDefault::unlitMaterial = nullptr;
    ObjectPtr<Material> ResourcesDefault::standardMaterial = nullptr;
    ObjectPtr<Material> ResourcesDefault::skyboxMaterial = nullptr;
    ObjectPtr<Mesh> ResourcesDefault::cubeMesh = nullptr;
    ObjectPtr<Mesh> ResourcesDefault::sphereMesh = nullptr;
    ObjectPtr<Mesh> ResourcesDefault::blenderMesh = nullptr;
    ObjectPtr<Texture2D> ResourcesDefault::whiteTexture = nullptr;

    ObjectPtr<Material> ResourcesDefault::GetUnlitMaterial()
    {
        return unlitMaterial;
    }
    ObjectPtr<Material> ResourcesDefault::GetStandardMaterial()
    {
        return standardMaterial;
    }
    ObjectPtr<Material> ResourcesDefault::GetSkyboxMaterial()
    {
        return skyboxMaterial;
    }
    ObjectPtr<Texture2D> ResourcesDefault::GetWhiteTexture()
    {
        return whiteTexture;
    }
    ObjectPtr<Mesh> ResourcesDefault::GetCubeMesh()
    {
        return cubeMesh;
    }
    ObjectPtr<Mesh> ResourcesDefault::GetSphereMesh()
    {
        return sphereMesh;
    }
    ObjectPtr<Mesh> ResourcesDefault::GetBlenderMesh()
    {
        return blenderMesh;
    }
    
    void ResourcesDefault::Initialize()
    {
# define ParsePath(path) "ResourcesDefault/"#path
        //着色器
        ObjectPtr<Shader> shadowMapShader = ShaderImport::Hlsl(ParsePath(Shaders/ShadowMap.hlsl));
        ObjectPtr<Shader> unlitShader = ShaderImport::Hlsl(ParsePath(Shaders/Unlit.hlsl));
        ObjectPtr<Shader> standardShader = ShaderImport::Hlsl(ParsePath(Shaders/Standard.hlsl));
        ObjectPtr<Shader> skyboxShader = ShaderImport::Hlsl(ParsePath(Shaders/Skybox.hlsl));
        //材质球
        unlitMaterial = Material::Create({
            {PassType::ForwardBase, unlitShader},
            {PassType::ForwardAdd, unlitShader},
            {PassType::ShadowCaster, shadowMapShader}
        });
        unlitMaterial->SetPassBlend(1, Blend::Additive);
        standardMaterial = Material::Create({
            {PassType::ForwardBase, standardShader},
            {PassType::ForwardAdd, standardShader},
            {PassType::ShadowCaster, shadowMapShader}
        });
        standardMaterial->SetPassBlend(1, Blend::Additive);
        skyboxMaterial = Material::Create({
            {PassType::ForwardBase, skyboxShader}
        });
        skyboxMaterial->SetPassZTest(0, ZTest::Background);
        //网格
        cubeMesh = MeshImport::Glb(ParsePath(Meshes/Cube.glb));
        sphereMesh = MeshImport::Glb(ParsePath(Meshes/Sphere.glb));
        blenderMesh = MeshImport::Glb(ParsePath(Meshes/Blender.glb));
        //贴图
        whiteTexture = Texture2D::Create(Color::white);
    }
}
