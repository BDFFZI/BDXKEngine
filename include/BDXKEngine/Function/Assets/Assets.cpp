#include "Assets.h"

#include "BDXKEngine/Platform/GL/Mesh/MeshImporter.h"
#include "BDXKEngine/Platform/GL/Shader/ShaderImporter.h"

namespace BDXKEngine
{
    ObjectPtr<Material> Assets::unlitMaterial = nullptr;
    ObjectPtr<Material> Assets::standardMaterial = nullptr;
    ObjectPtr<Material> Assets::skyboxMaterial = nullptr;
    ObjectPtr<Mesh> Assets::cubeMesh = nullptr;
    ObjectPtr<Mesh> Assets::sphereMesh = nullptr;
    ObjectPtr<Mesh> Assets::blenderMesh = nullptr;
    ObjectPtr<Texture2D> Assets::whiteTexture = nullptr;

    ObjectPtr<Material> Assets::GetUnlitMaterial()
    {
        return unlitMaterial;
    }
    ObjectPtr<Material> Assets::GetStandardMaterial()
    {
        return standardMaterial;
    }
    ObjectPtr<Material> Assets::GetSkyboxMaterial()
    {
        return skyboxMaterial;
    }
    ObjectPtr<Texture2D> Assets::GetWhiteTexture()
    {
        return whiteTexture;
    }
    ObjectPtr<Mesh> Assets::GetCubeMesh()
    {
        return cubeMesh;
    }
    ObjectPtr<Mesh> Assets::GetSphereMesh()
    {
        return sphereMesh;
    }
    ObjectPtr<Mesh> Assets::GetBlenderMesh()
    {
        return blenderMesh;
    }


    void Assets::Initialize(Window* window)
    {
# define ParsePath(path) "Assets/BDXKEngine/"#path
        //材质球
        ObjectPtr<Shader> shadowMapShader = ShaderImporter::Import(ParsePath(Shaders/ShadowMap.hlsl));
        auto unlitShader = ShaderImporter::Import(ParsePath(Shaders/Unlit.hlsl));
        auto unlitShaderAdditive = ShaderImporter::Import(ParsePath(Shaders/Unlit.hlsl));
        unlitShaderAdditive->SetBlend(Blend::Additive);
        unlitMaterial = Material::Create({
            {unlitShader, PassType::ForwardBase},
            {unlitShaderAdditive, PassType::ForwardAdd},
            {shadowMapShader, PassType::ShadowCaster}
        });
        auto standardShader = ShaderImporter::Import(ParsePath(Shaders/Standard.hlsl));
        auto standardShaderAdditive = ShaderImporter::Import(ParsePath(Shaders/Standard.hlsl));
        standardShaderAdditive->SetBlend(Blend::Additive);
        standardMaterial = Material::Create({
            {standardShader, PassType::ForwardBase},
            {standardShaderAdditive, PassType::ForwardAdd},
            {shadowMapShader, PassType::ShadowCaster}
        });
        ObjectPtr<Shader> skyboxShader = ShaderImporter::Import(ParsePath(Shaders/Skybox.hlsl));
        ZTest ztest = {};
        ztest.write = false;
        ztest.operation = ZTest::Operation::Always;
        skyboxShader->SetZTest(ztest);
        skyboxMaterial = Material::Create({
            {
                skyboxShader,
                PassType::ForwardBase
            }
        });
        //网格
        cubeMesh = MeshImporter::Import(ParsePath(Meshes/Cube.glb));
        sphereMesh = MeshImporter::Import(ParsePath(Meshes/Sphere.glb));
        blenderMesh = MeshImporter::Import(ParsePath(Meshes/Blender.glb));
        //贴图
        whiteTexture = Texture2D::Create(Color::white);

        window->AddDestroyEvent([]()
        {
            Object::DestroyImmediate(unlitMaterial);
            Object::DestroyImmediate(whiteTexture = nullptr);
            Object::DestroyImmediate(cubeMesh = nullptr);
            Object::DestroyImmediate(sphereMesh = nullptr);
            Object::DestroyImmediate(blenderMesh = nullptr);
        });
    }
}
