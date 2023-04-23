#include "RenderGameObjectMenu.h"
#include "Core/Editor/EditorMenu/GameObjectMenu.h"
#include "Core/Runtime/Base/Object/Serialization/ObjectSerialization.h"
#include "Render/Runtime/Renderer/Core/Camera.h"
#include "Render/Editor/AssetImporter/RenderAssets.h"

namespace BDXKEditor
{
    ObjectPtr<GameObject> RenderGameObjectMenu::CreateCamera()
    {
        ObjectPtr<GameObject> gameObject = GameObjectMenu::NewGameObject("Camera");
        Component::Create<Camera>(gameObject);
        return gameObject;
    }
    ObjectPtr<GameObject> RenderGameObjectMenu::CreateLight(const LightType lightType, const char* name)
    {
        const ObjectPtr<GameObject> gameObject = GameObjectMenu::NewGameObject(name);
        const ObjectPtr<Light> light = Component::Create<Light>(gameObject);
        light->SetColor(Color::white);
        light->SetIntensity(0.5f);
        light->SetType(lightType);
        return gameObject;
    }
    ObjectPtr<GameObject> RenderGameObjectMenu::CreateDirectionalLight()
    {
        return CreateLight(LightType::Directional, "DirectionalLight");
    }
    ObjectPtr<GameObject> RenderGameObjectMenu::CreatePointLight()
    {
        return CreateLight(LightType::Point, "PointLight");
    }
    ObjectPtr<GameObject> RenderGameObjectMenu::CreateBlender()
    {
        auto cube = ObjectSerialization::Clone(RenderAssets::GetBlenderModel());
        cube->SetName("Blender");
        cube->SetScene(Scene::GetActiveScene());
        return cube;
    }
    ObjectPtr<GameObject> RenderGameObjectMenu::CreateCube()
    {
        auto cube = ObjectSerialization::Clone(RenderAssets::GetCubeModel());
        cube->SetName("Cube");
        cube->SetScene(Scene::GetActiveScene());
        return cube;
    }
    ObjectPtr<GameObject> RenderGameObjectMenu::CreateSphere()
    {
        auto sphere = ObjectSerialization::Clone(RenderAssets::GetSphereModel());
        sphere->SetName("Sphere");
        sphere->SetScene(Scene::GetActiveScene());
        return sphere;
    }
    ObjectPtr<GameObject> RenderGameObjectMenu::CreatePlane()
    {
        auto plane = ObjectSerialization::Clone(RenderAssets::GetCubeModel());
        plane->SetName("Plane");
        plane->SetScene(Scene::GetActiveScene());
        plane->SetLocalScale({10, 0.01f, 10});
        plane->SetLocalPosition({0, -0.005f, 0});
        return plane;
    }
}
