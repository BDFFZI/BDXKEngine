#include "Scene.h"
#include "BDXKEngine/Function/Resources/Resources.h"
#include "BDXKEngine/Function/Resources/ResourcesDefault.h"
#include "BDXKEngine/Function/Time/Time.h"
#include "BDXKEngine/Platform/Serialization/Serialization.h"
#include "BDXKEngine/Framework/Physics/Collider/BoxCollider.h"
#include "BDXKEngine/Framework/Physics/Collider/SphereCollider.h"
#include "Renderer/MeshRenderer.h"
#include "Renderer/Core/Camera.h"
#include "Renderer/Core/Light.h"

namespace BDXKEngine
{
    ObjectPtr<GameObject> SceneDefault::CreateObject3D(const ObjectPtr<Mesh>& mesh, const char* name, Color color)
    {
        //创建物体
        const ObjectPtr<GameObject> gameObject = GameObject::Create(name);
        const ObjectPtr<MeshRenderer> meshRenderer = Component::Create<MeshRenderer>(gameObject);
        meshRenderer->SetMesh(mesh);
        meshRenderer->SetMaterial(ResourcesDefault::GetStandardMaterial());

        return gameObject;
    }
    ObjectPtr<GameObject> SceneDefault::CreateCube(const char* name, Color color)
    {
        auto cube = CreateObject3D(ResourcesDefault::GetCubeMesh(), name, color);
        Component::Create<BoxCollider>(cube);
        return cube;
    }
    ObjectPtr<GameObject> SceneDefault::CreatePlane(const char* name, Color color)
    {
        ObjectPtr<GameObject> plane = CreateCube(name, color);
        Component::Create<BoxCollider>(plane);
        plane->SetLocalScale({10, 0.01f, 10});
        plane->SetLocalPosition({0, -0.005f, 0});
        return plane;
    }
    ObjectPtr<GameObject> SceneDefault::CreateSphere(const char* name, Color color)
    {
        auto sphere = CreateObject3D(ResourcesDefault::GetSphereMesh(), name, color);
        Component::Create<SphereCollider>(sphere);
        return sphere;
    }
    ObjectPtr<GameObject> SceneDefault::CreateLight(LightType lightType, const char* name)
    {
        const ObjectPtr<GameObject> gameObject = GameObject::Create(name);
        const ObjectPtr<Light> light = Component::Create<Light>(gameObject);
        light->SetColor(Color::white);
        light->SetIntensity(0.5f);
        light->SetType(lightType);
        return gameObject;
    }
    ObjectPtr<GameObject> SceneDefault::CreateDirectionalLight(const char* name)
    {
        return CreateLight(LightType::Directional, name);
    }
    ObjectPtr<GameObject> SceneDefault::CreatePointLight(const char* name)
    {
        return CreateLight(LightType::Point, name);
    }
    ObjectPtr<GameObject> SceneDefault::CreateCamera(const char* name)
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

        const ObjectPtr<GameObject> sun = SceneDefault::CreateDirectionalLight("Sun");
        const ObjectPtr<GameObject> ground = SceneDefault::CreatePlane("Ground");
        const ObjectPtr<GameObject> sphere = SceneDefault::CreateSphere("Sphere");
        const ObjectPtr<GameObject> camera = SceneDefault::CreateCamera("Camera");
        sun->SetLocalEulerAngles({45, -45, 0});
        sphere->SetLocalPosition({0, 0.5f, 0});
        camera->SetLocalPosition({0, 1, -10});
    }

    void Scene::Load(const std::string& sceneName, bool retainPersistent)
    {
        GameObject::Clear();
        const ObjectPtr<Scene> scene = Resources::Load<Scene>(sceneName, retainPersistent);
        Time::RenewFrame();
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
