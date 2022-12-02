#include "Scene.h"
#include "BDXKEngine/Function/Assets/Assets.h"
#include "BDXKEngine/Platform/Resources/Resources.h"
#include "Renderer/MeshRenderer.h"
#include "Renderer/Core/Camera.h"
#include "Renderer/Core/Light.h"

namespace BDXKEngine
{
    ObjectPtr<GameObject> Creation::CreateObject3D(const ObjectPtr<Mesh>& mesh, const char* name, Color color)
    {
        //创建材质球
        const ObjectPtr<Material> material = Resources::Clone(Assets::GetStandardMaterial());
        material->SetTexture2D(0, Texture2D::Create(color));
        //创建物体
        const ObjectPtr<GameObject> gameObject = GameObject::Create(name);
        const ObjectPtr<MeshRenderer> meshRenderer = Component::Create<MeshRenderer>(gameObject);
        meshRenderer->SetMesh(mesh);
        meshRenderer->SetMaterial(material);

        return gameObject;
    }
    ObjectPtr<GameObject> Creation::CreateCube(const char* name, Color color)
    {
        return CreateObject3D(Assets::GetCubeMesh(), name, color);
    }
    ObjectPtr<GameObject> Creation::CreatePlane(const char* name, Color color)
    {
        ObjectPtr<GameObject> plane = CreateCube(name, color);
        plane->SetLocalScale({10, 0.01f, 10});
        plane->SetLocalPosition({0, -0.005f, 0});
        return plane;
    }
    ObjectPtr<GameObject> Creation::CreateSphere(const char* name, Color color)
    {
        return CreateObject3D(Assets::GetSphereMesh(), name, color);
    }
    ObjectPtr<GameObject> Creation::CreateLight(LightType lightType, const char* name)
    {
        const ObjectPtr<GameObject> gameObject = GameObject::Create(name);
        const ObjectPtr<Light> light = Component::Create<Light>(gameObject);
        light->SetColor(Color::white);
        light->SetIntensity(0.5f);
        light->SetType(lightType);
        return gameObject;
    }
    ObjectPtr<GameObject> Creation::CreateDirectionalLight(const char* name)
    {
        return CreateLight(LightType::Directional, name);
    }
    ObjectPtr<GameObject> Creation::CreatePointLight(const char* name)
    {
        return CreateLight(LightType::Point, name);
    }
    ObjectPtr<GameObject> Creation::CreateCamera(const char* name)
    {
        ObjectPtr<GameObject> gameObject = GameObject::Create(name);
        Component::Create<Camera>(gameObject);
        return gameObject;
    }

    void Scene::LoadDefault()
    {
        RenderSettings::SetSingleton(RenderSettings::Create());
        const ObjectPtr<GameObject> sun = Creation::CreateDirectionalLight("Sun");
        const ObjectPtr<GameObject> ground = Creation::CreatePlane("Ground");
        const ObjectPtr<GameObject> sphere = Creation::CreateSphere("Sphere");
        const ObjectPtr<GameObject> camera = Creation::CreateCamera("Camera");
        sun->SetLocalEulerAngles({45, -45, 0});
        sphere->SetLocalPosition({0, 0.5f, 0});
        camera->SetLocalPosition({0, 1, -10});
    }
    void Scene::Load(const ObjectPtr<Scene>& path)
    {
        RenderSettings::SetSingleton(path->renderSettings);
        for (const auto& item : path->gameObjects)
        {
            Resources::Clone(item);
            DestroyImmediate(item);
        }
    }
    ObjectPtr<Scene> Scene::GetCurrentScene()
    {
        ObjectPtr scene = new Scene{};
        Instantiate(scene);
        scene->renderSettings = &RenderSettings::GetSingleton();
        for (const auto& item : ScriptableObject::FindScriptableObjectsOfType<GameObject>())
        {
            if (Resources::IsResource(item) == false && item->GetParent().IsNull())
                scene->gameObjects.emplace_back(item);
        }
        return scene;
    }

    std::vector<ObjectPtr<GameObject>>& Scene::GameObjects()
    {
        return gameObjects;
    }
    void Scene::Transfer(Transferer& transferer)
    {
        Object::Transfer(transferer);

        TransferFieldInfo(renderSettings);
        TransferFieldInfo(gameObjects);
    }
}
