#include "CreateGameObjectMenu.h"

#include "CreateAssetMenu.h"
#include "BDXKEditor/Function/AssetsBuiltIn.h"
#include "BDXKEngine/Framework/Scene.h"
#include "BDXKEngine/Framework/Physics/Collider/BoxCollider.h"
#include "BDXKEngine/Framework/Physics/Collider/SphereCollider.h"
#include "BDXKEngine/Framework/Renderer/MeshRenderer.h"
#include "BDXKEngine/Framework/Renderer/Core/Camera.h"
#include "BDXKEngine/Framework/Renderer/Core/Light.h"

namespace BDXKEditor
{
    ObjectPtr<GameObject> CreateGameObjectMenu::CreateGameObject()
    {
        return GameObject::Create();
    }

    ObjectPtr<GameObject> CreateGameObjectMenu::CreateCamera()
    {
        ObjectPtr<GameObject> gameObject = GameObject::Create("Camera");
        Component::Create<Camera>(gameObject);
        return gameObject;
    }

    ObjectPtr<GameObject> CreateGameObjectMenu::CreateLight(LightType lightType, const char* name)
    {
        const ObjectPtr<GameObject> gameObject = GameObject::Create(name);
        const ObjectPtr<Light> light = Component::Create<Light>(gameObject);
        light->SetColor(Color::white);
        light->SetIntensity(0.5f);
        light->SetType(lightType);
        return gameObject;
    }
    ObjectPtr<GameObject> CreateGameObjectMenu::CreateDirectionalLight()
    {
        return CreateLight(LightType::Directional, "DirectionalLight");
    }
    ObjectPtr<GameObject> CreateGameObjectMenu::CreatePointLight()
    {
        return CreateLight(LightType::Point, "PointLight");
    }

    ObjectPtr<GameObject> CreateGameObjectMenu::CreateObject3D(const ObjectPtr<Mesh>& mesh, const char* name, Color color)
    {
        //创建物体
        const ObjectPtr<GameObject> gameObject = GameObject::Create(name);
        const ObjectPtr<MeshRenderer> meshRenderer = Component::Create<MeshRenderer>(gameObject);
        meshRenderer->SetMesh(mesh);
        meshRenderer->SetMaterial(CreateAssetMenu::CreateStandardMaterial());
        return gameObject;
    }
    ObjectPtr<GameObject> CreateGameObjectMenu::CreateCube()
    {
        auto cube = CreateObject3D(AssetsBuiltIn::GetCubeMesh(), "Cube");
        Component::Create<BoxCollider>(cube);
        return cube;
    }
    ObjectPtr<GameObject> CreateGameObjectMenu::CreateSphere()
    {
        auto sphere = CreateObject3D(AssetsBuiltIn::GetSphereMesh(), "Sphere");
        Component::Create<SphereCollider>(sphere);
        return sphere;
    }
    ObjectPtr<GameObject> CreateGameObjectMenu::CreatePlane()
    {
        auto plane = CreateObject3D(AssetsBuiltIn::GetCubeMesh(), "Plane");
        plane->SetLocalScale({10, 0.01f, 10});
        plane->SetLocalPosition({0, -0.005f, 0});
        Component::Create<BoxCollider>(plane);
        return plane;
    }
}
