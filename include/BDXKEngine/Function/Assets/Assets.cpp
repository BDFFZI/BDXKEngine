#include "Assets.h"

#include "BDXKEngine/Platform/GL/Mesh/MeshImporter.h"
#include "BDXKEngine/Platform/GL/Shader/ShaderImporter.h"

namespace BDXKEngine
{
    ObjectPtr<Material> Assets::shadowMapMaterial = nullptr;
    ObjectPtr<Material> Assets::skyboxMaterial = nullptr;
    ObjectPtr<Material> Assets::blitMaterial = nullptr;
    ObjectPtr<Texture2D> Assets::whiteTexture = nullptr;
    ObjectPtr<Mesh> Assets::cubeMesh = nullptr;
    ObjectPtr<Mesh> Assets::sphereMesh = nullptr;
    ObjectPtr<Mesh> Assets::blenderMesh = nullptr;

    ObjectPtr<Material> Assets::GetShadowMapMaterial()
    {
        return shadowMapMaterial;
    }
    ObjectPtr<Material> Assets::GetSkyboxMaterial()
    {
        return skyboxMaterial;
    }
    ObjectPtr<Material> Assets::GetBlitMaterial()
    {
        return blitMaterial;
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
        //绘制阴影贴图
        shadowMapMaterial = Material::Create({
            {
                ShaderImporter::Import("Shaders/ShadowMap.hlsl"),
                ShaderType::ForwardBase
            }
        });
        //绘制天空盒
        const ObjectPtr<Shader> skyboxShader = ShaderImporter::Import("Shaders/Skybox.hlsl");
        ZTest ztest = {};
        ztest.write = false;
        ztest.operation = ZTest::Operation::Always;
        skyboxShader->SetZTest(ztest);
        skyboxMaterial = Material::Create({
            {
                skyboxShader,
                ShaderType::ForwardBase
            }
        });
        //位块传输
        blitMaterial = Material::Create({
            {
                ShaderImporter::Import("Shaders/Blit.hlsl"),
                ShaderType::ForwardBase
            }
        });
        //贴图
        whiteTexture = Texture2D::Create(Color::white);
        //网格
        cubeMesh = MeshImporter::Import("Meshs/Cube.glb");
        sphereMesh = MeshImporter::Import("Meshs/Sphere.glb");
        blenderMesh = MeshImporter::Import("Meshs/Blender.glb");

        window->AddDestroyEvent([]()
        {
            shadowMapMaterial = nullptr;
            skyboxMaterial = nullptr;
            blitMaterial = nullptr;
            whiteTexture = nullptr;
            cubeMesh = nullptr;
            sphereMesh = nullptr;
            blenderMesh = nullptr;
        });
    }
}
