#include "Scene.h"
#include "BDXKEngine/Platform/GL/Mesh/MeshImport.h"
#include "BDXKEngine/Platform/GL/Shader/ShaderImport.h"
#include "BDXKEngine/Platform/Resources/Resources.h"
#include "Renderer/MeshRenderer.h"
#include "Renderer/Core/Camera.h"
#include "Renderer/Core/Light.h"

namespace BDXKEngine
{
    ObjectPtr<Material> PresetResources::unlitMaterial = nullptr;
    ObjectPtr<Material> PresetResources::standardMaterial = nullptr;
    ObjectPtr<Material> PresetResources::skyboxMaterial = nullptr;
    ObjectPtr<Mesh> PresetResources::cubeMesh = nullptr;
    ObjectPtr<Mesh> PresetResources::sphereMesh = nullptr;
    ObjectPtr<Mesh> PresetResources::blenderMesh = nullptr;
    ObjectPtr<Texture2D> PresetResources::whiteTexture = nullptr;

    ObjectPtr<Material> PresetResources::GetUnlitMaterial()
    {
        return unlitMaterial;
    }
    ObjectPtr<Material> PresetResources::GetStandardMaterial()
    {
        return standardMaterial;
    }
    ObjectPtr<Material> PresetResources::GetSkyboxMaterial()
    {
        return skyboxMaterial;
    }
    ObjectPtr<Texture2D> PresetResources::GetWhiteTexture()
    {
        return whiteTexture;
    }
    ObjectPtr<Mesh> PresetResources::GetCubeMesh()
    {
        return cubeMesh;
    }
    ObjectPtr<Mesh> PresetResources::GetSphereMesh()
    {
        return sphereMesh;
    }
    ObjectPtr<Mesh> PresetResources::GetBlenderMesh()
    {
        return blenderMesh;
    }


    void PresetResources::Create()
    {
# define ParsePath(path) Resources::GetRootPath()+"/BDXKEngine/"#path
        //材质球
        ObjectPtr<Shader> shadowMapShader = ShaderImport::Hlsl(ParsePath(Shaders/ShadowMap.hlsl));
        auto unlitShader = ShaderImport::Hlsl(ParsePath(Shaders/Unlit.hlsl));
        auto unlitShaderAdditive = ShaderImport::Hlsl(ParsePath(Shaders/Unlit.hlsl));
        unlitShaderAdditive->SetBlend(Blend::Additive);
        unlitMaterial = Material::Create({
            {unlitShader, PassType::ForwardBase},
            {unlitShaderAdditive, PassType::ForwardAdd},
            {shadowMapShader, PassType::ShadowCaster}
        });
        auto standardShader = ShaderImport::Hlsl(ParsePath(Shaders/Standard.hlsl));
        auto standardShaderAdditive = ShaderImport::Hlsl(ParsePath(Shaders/Standard.hlsl));
        standardShaderAdditive->SetBlend(Blend::Additive);
        standardMaterial = Material::Create({
            {standardShader, PassType::ForwardBase},
            {standardShaderAdditive, PassType::ForwardAdd},
            {shadowMapShader, PassType::ShadowCaster}
        });
        ObjectPtr<Shader> skyboxShader = ShaderImport::Hlsl(ParsePath(Shaders/Skybox.hlsl));
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
        cubeMesh = MeshImport::Glb(ParsePath(Meshes/Cube.glb));
        sphereMesh = MeshImport::Glb(ParsePath(Meshes/Sphere.glb));
        blenderMesh = MeshImport::Glb(ParsePath(Meshes/Blender.glb));
        //贴图
        whiteTexture = Texture2D::Create(Color::white);
    }

    ObjectPtr<GameObject> PresetGameObject::CreateObject3D(const ObjectPtr<Mesh>& mesh, const char* name, Color color)
    {
        //创建材质球
        const ObjectPtr<Material> material = Resources::Clone(PresetResources::GetStandardMaterial());
        material->SetTexture2D(0, Texture2D::Create(color));
        //创建物体
        const ObjectPtr<GameObject> gameObject = GameObject::Create(name);
        const ObjectPtr<MeshRenderer> meshRenderer = Component::Create<MeshRenderer>(gameObject);
        meshRenderer->SetMesh(mesh);
        meshRenderer->SetMaterial(material);

        return gameObject;
    }
    ObjectPtr<GameObject> PresetGameObject::CreateCube(const char* name, Color color)
    {
        return CreateObject3D(PresetResources::GetCubeMesh(), name, color);
    }
    ObjectPtr<GameObject> PresetGameObject::CreatePlane(const char* name, Color color)
    {
        ObjectPtr<GameObject> plane = CreateCube(name, color);
        plane->SetLocalScale({10, 0.01f, 10});
        plane->SetLocalPosition({0, -0.005f, 0});
        return plane;
    }
    ObjectPtr<GameObject> PresetGameObject::CreateSphere(const char* name, Color color)
    {
        return CreateObject3D(PresetResources::GetSphereMesh(), name, color);
    }
    ObjectPtr<GameObject> PresetGameObject::CreateLight(LightType lightType, const char* name)
    {
        const ObjectPtr<GameObject> gameObject = GameObject::Create(name);
        const ObjectPtr<Light> light = Component::Create<Light>(gameObject);
        light->SetColor(Color::white);
        light->SetIntensity(0.5f);
        light->SetType(lightType);
        return gameObject;
    }
    ObjectPtr<GameObject> PresetGameObject::CreateDirectionalLight(const char* name)
    {
        return CreateLight(LightType::Directional, name);
    }
    ObjectPtr<GameObject> PresetGameObject::CreatePointLight(const char* name)
    {
        return CreateLight(LightType::Point, name);
    }
    ObjectPtr<GameObject> PresetGameObject::CreateCamera(const char* name)
    {
        ObjectPtr<GameObject> gameObject = GameObject::Create(name);
        Component::Create<Camera>(gameObject);
        return gameObject;
    }

    void Scene::LoadDefault()
    {
        GameObject::Clear();
        PresetResources::Create();
        const ObjectPtr<RenderSettings> renderSettings = Create<RenderSettings>();
        renderSettings->SetSkyboxMaterial(PresetResources::GetSkyboxMaterial());
        renderSettings->SetUnlitMaterial(PresetResources::GetUnlitMaterial());

        const ObjectPtr<GameObject> sun = PresetGameObject::CreateDirectionalLight("Sun");
        const ObjectPtr<GameObject> ground = PresetGameObject::CreatePlane("Ground");
        const ObjectPtr<GameObject> sphere = PresetGameObject::CreateSphere("Sphere");
        const ObjectPtr<GameObject> camera = PresetGameObject::CreateCamera("Camera");
        sun->SetLocalEulerAngles({45, -45, 0});
        sphere->SetLocalPosition({0, 0.5f, 0});
        camera->SetLocalPosition({0, 1, -10});
    }

    void Scene::Load(const std::string& sceneName, bool asResource)
    {
        GameObject::Clear();

        const ObjectPtr<Scene> scene = Resources::LoadOf<Scene>(sceneName);
        if (asResource == false)
            for (const auto& item : scene->gameObjects)
            {
                Resources::Clone(item);
                DestroyImmediate(item);
            }
    }
    void Scene::Save(const std::string& sceneName)
    {
        const ObjectPtr<Scene> newScene = GetCurrentScene();
        newScene->SetName(sceneName);
        Resources::Save(sceneName, newScene);
    }
    ObjectPtr<Scene> Scene::GetCurrentScene()
    {
        ObjectPtr scene = new Scene{};
        Instantiate(scene);
        scene->renderSettings = RenderSettings::GetSingleton();
        for (const auto& item : GameObject::GetGameObjects())
        {
            if (item->GetParent().IsNull())
                scene->gameObjects.emplace_back(item);
        }
        return scene;
    }
    
    void Scene::Transfer(Transferer& transferer)
    {
        Object::Transfer(transferer);

        TransferFieldInfo(renderSettings);
        TransferFieldInfo(gameObjects);
    }
}
