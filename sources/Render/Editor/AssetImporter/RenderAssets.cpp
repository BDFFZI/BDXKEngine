#include "RenderAssets.h"
#include <filesystem>
#include "Core/Editor/Core/Assets/AssetDatabase.h"
#include "Core/Editor/Setting/PackageSetting.h"

namespace BDXKEditor
{
    const ObjectPtr<Shader>& RenderAssets::GetStandardShader()
    {
        return standardShader;
    }
    const ObjectPtr<Shader>& RenderAssets::GetUnlitShader()
    {
        return unlitShader;
    }
    const ObjectPtr<Shader>& RenderAssets::GetShadowMapShader()
    {
        return shadowMapShader;
    }
    const ObjectPtr<Shader>& RenderAssets::GetSkyboxShader()
    {
        return skyboxShader;
    }

    const ObjectPtr<GameObject>& RenderAssets::GetCubeModel()
    {
        return cubeModel;
    }
    const ObjectPtr<GameObject>& RenderAssets::GetSphereModel()
    {
        return sphereModel;
    }
    const ObjectPtr<GameObject>& RenderAssets::GetBlenderModel()
    {
        return blenderModel;
    }

    void RenderAssets::OnEngineStart()
    {
        const std::filesystem::path path = PackageSetting::GetAssetsPath("Render");
        if (path.empty())
            throw std::exception("无法获取到渲染包素材,请检查包信息是否填写正常。");

# define ParsePath(filePath) std::filesystem::path{path}.concat("/"#filePath)
        //着色器
        standardShader = AssetDatabase::ReLoad<Shader>(ParsePath(Shaders/Standard.hlsl));
        unlitShader = AssetDatabase::ReLoad<Shader>(ParsePath(Shaders/Unlit.hlsl));
        shadowMapShader = AssetDatabase::ReLoad<Shader>(ParsePath(Shaders/ShadowMap.hlsl));
        skyboxShader = AssetDatabase::ReLoad<Shader>(ParsePath(Shaders/Skybox.hlsl));
        //模型
        cubeModel = AssetDatabase::ReLoad<GameObject>(ParsePath(Meshes/Cube.glb));
        sphereModel = AssetDatabase::ReLoad<GameObject>(ParsePath(Meshes/Sphere.glb));
        blenderModel = AssetDatabase::ReLoad<GameObject>(ParsePath(Meshes/Blender.glb));
    }
}
