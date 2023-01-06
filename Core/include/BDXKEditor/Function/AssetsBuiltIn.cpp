#include "AssetsBuiltIn.h"

#include <filesystem>

#include "Assets.h"

namespace BDXKEditor
{
    ObjectPtr<Mesh> AssetsBuiltIn::cubeMesh = nullptr;
    ObjectPtr<Mesh> AssetsBuiltIn::sphereMesh = nullptr;
    ObjectPtr<Mesh> AssetsBuiltIn::blenderMesh = nullptr;
    ObjectPtr<Shader> AssetsBuiltIn::standardShader = nullptr;
    ObjectPtr<Shader> AssetsBuiltIn::unlitShader = nullptr;
    ObjectPtr<Shader> AssetsBuiltIn::shadowMapShader = nullptr;
    ObjectPtr<Shader> AssetsBuiltIn::skyboxShader = nullptr;

    ObjectPtr<Mesh>& AssetsBuiltIn::GetCubeMesh()
    {
        return cubeMesh;
    }
    ObjectPtr<Mesh>& AssetsBuiltIn::GetSphereMesh()
    {
        return sphereMesh;
    }
    ObjectPtr<Mesh>& AssetsBuiltIn::GetBlenderMesh()
    {
        return blenderMesh;
    }
    ObjectPtr<Shader>& AssetsBuiltIn::GetStandardShader()
    {
        return standardShader;
    }
    ObjectPtr<Shader>& AssetsBuiltIn::GetUnlitShader()
    {
        return unlitShader;
    }
    ObjectPtr<Shader>& AssetsBuiltIn::GetShadowMapShader()
    {
        return shadowMapShader;
    }
    ObjectPtr<Shader>& AssetsBuiltIn::GetSkyboxShader()
    {
        return skyboxShader;
    }

    void AssetsBuiltIn::Initialize()
    {
        if (std::filesystem::exists(Assets::GetRootPath() + "/build-in") == false)
            throw std::exception("无法获取到编辑器的内置资源(阅读Core/build-in/ReadMe.md)");

# define ParsePath(path) "build-in/"#path
        //网格
        cubeMesh = Assets::Load<Mesh>(ParsePath(Meshes/Cube.glb));
        sphereMesh = Assets::Load<Mesh>(ParsePath(Meshes/Sphere.glb));
        blenderMesh = Assets::Load<Mesh>(ParsePath(Meshes/Blender.glb));
        //着色器
        shadowMapShader = Assets::Load<Shader>(ParsePath(Shaders/ShadowMap.hlsl));
        unlitShader = Assets::Load<Shader>(ParsePath(Shaders/Unlit.hlsl));
        standardShader = Assets::Load<Shader>(ParsePath(Shaders/Standard.hlsl));
        skyboxShader = Assets::Load<Shader>(ParsePath(Shaders/Skybox.hlsl));
    }
}
