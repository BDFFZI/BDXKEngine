#include "Scene.h"
#include "BDXKEngine/Function/Resources/Resources.h"
#include "BDXKEngine/Function/Resources/ResourcesDefault.h"
#include "BDXKEngine/Platform/Serialization/Serialization.h"
#include "Renderer/MeshRenderer.h"
#include "Renderer/Core/Camera.h"
#include "Renderer/Core/Light.h"

namespace BDXKEngine
{
    ObjectPtr<GameObject> PresetGameObject::CreateObject3D(const ObjectPtr<Mesh>& mesh, const char* name, Color color)
    {
        //创建材质球
        const ObjectPtr<Material> material = Serialization::Clone(ResourcesDefault::GetStandardMaterial());
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
        return CreateObject3D(ResourcesDefault::GetCubeMesh(), name, color);
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
        return CreateObject3D(ResourcesDefault::GetSphereMesh(), name, color);
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
        RenderSettings::SetSkyboxMaterial(ResourcesDefault::GetSkyboxMaterial());
        RenderSettings::SetUnlitMaterial(ResourcesDefault::GetUnlitMaterial());

        const ObjectPtr<GameObject> sun = PresetGameObject::CreateDirectionalLight("Sun");
        const ObjectPtr<GameObject> ground = PresetGameObject::CreatePlane("Ground");
        const ObjectPtr<GameObject> sphere = PresetGameObject::CreateSphere("Sphere");
        const ObjectPtr<GameObject> camera = PresetGameObject::CreateCamera("Camera");
        sun->SetLocalEulerAngles({45, -45, 0});
        sphere->SetLocalPosition({0, 0.5f, 0});
        camera->SetLocalPosition({0, 1, -10});
    }

    void Scene::Load(const std::string& sceneName, bool keepPersistent)
    {
        GameObject::Clear();

        const ObjectPtr<Scene> scene = Resources::Load<Scene>(sceneName);
        if (keepPersistent == false)
            for (const auto& item : scene->gameObjects)
            {
                Serialization::Clone(item);
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
