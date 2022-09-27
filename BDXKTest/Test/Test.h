#pragma once
#include <BDXKEngine.h>
#include "Editor/MaterialHUD.h"
#include "Editor/CreationMenu.h"
#include "CameraController.h"
#include "CreateCube.h"

namespace Assembly
{
    using namespace BDXKEditor;

    inline void CreateDefaultScene()
    {
        const ObjectPtr<GameObject> camera = CreationMenu::Camera(L"摄像机");
        camera->AddComponent<CameraController>();
        camera->AddComponent<SceneHUD>();
        ObjectPtr<GameObject> sun = CreationMenu::Light::DirectionalLight(L"太阳");
        ObjectPtr<GameObject> ground = CreationMenu::Object3D::Plane(L"地面");
        ObjectPtr<GameObject> sphere = CreationMenu::Object3D::Sphere(L"球");
        //太阳
        {
            ObjectPtr<Transform> transform = sun->GetTransform();
            transform->SetLocalEulerAngles({45, -45, 0});
        }
        //球的展示位置
        {
            ObjectPtr<Transform> transform = sphere->GetTransform();
            transform->SetLocalPosition({0, 0.5f, 0});
        }
        //增加控制功能并调整位置
        {
            ObjectPtr<MaterialHUD> materialHUD = camera->AddComponent<MaterialHUD>();
            materialHUD->materials.push_back(sphere->GetComponent<MeshRenderer>()->GetMaterial());
            camera->GetTransform()->SetLocalPosition({0, 1, -10});
        }

        ObjectPtr<GameObject> blender = CreationMenu::Object3D::CreateObject3D(Resources::GetBlenderMesh(), L"Blender");
        blender->GetTransform()->SetLocalPosition({0, 3.0f, 0});
    }

    inline void TestTransparency()
    {
        const ObjectPtr<Camera> camera = GameObject::Find(L"摄像机")->GetComponent<Camera>();
        camera->GetGameObject()->AddComponent<CreateCube>();
    }

    void TestLight()
    {
        ObjectPtr<GameObject> aureole = GameObject::Create(L"光环");
        {
            ObjectPtr<Transform> transform = aureole->GetTransform();
            transform->SetParent(GameObject::Find(L"摄像机")->GetTransform());
            transform->SetLocalPosition({0, 0, 0.7f});

            ObjectPtr<Animator> animator = aureole->AddComponent<Animator>();
            animator->SetAnimation(AnimatorClip::Create([](ObjectPtr<Transform> transform)
            {
                Vector3 position = transform->GetLocalPosition();
                position.x = std::cosf(Time::GetRealtimeSinceStartup()) * 0.3f;
                position.y = std::sinf(Time::GetRealtimeSinceStartup()) * 0.3f;
                transform->SetLocalPosition(position);

                Vector3 eulerAngles = transform->GetLocalEulerAngles();
                eulerAngles.z -= 90 * Time::GetDeltaTime();
                transform->SetLocalEulerAngles(eulerAngles);
            }));
        }

        ObjectPtr<GameObject> sphere = CreationMenu::Object3D::Sphere(L"小球");
        ObjectPtr<Light> sphere_light;
        {
            ObjectPtr<Transform> transform = sphere->GetTransform();
            transform->SetParent(aureole->GetTransform());
            transform->SetLocalScale({0.1f, 0.1f, 0.1f});

            sphere_light = CreationMenu::Light::PointLight(L"红色点光源")->GetComponent<Light>();
            sphere_light->SetLightType(LightType::Point);
            sphere_light->SetColor(Color::red);
            sphere_light->SetIntensity(0.5f);
            sphere_light->GetTransform()->SetParent(sphere->GetTransform());
        }

        ObjectPtr<GameObject> cube = CreationMenu::Object3D::Cube(L"棍子");
        {
            ObjectPtr<Transform> transform = cube->GetTransform();
            transform->SetParent(aureole->GetTransform());
            transform->SetLocalScale({0.2f, 0.05f, 0.05f});
        }
    }

    void TestShadow()
    {
        ObjectPtr<GameObject> camera = GameObject::Find(L"摄像机");
        ObjectPtr<Light> pointlight = GameObject::Find(L"红色点光源")->GetComponent<Light>();
        //GraphicsSettings::skybox = pointlight->shadowMapCube;

        //ObjectPtr<GameObject> cube = CreationMenu::Object3D::Cube(L"阴影贴图显示器");
        ////阴影贴图显示器
        //{
        //	ObjectPtr<Transform> transform = cube->GetTransform();
        //	transform->SetParent(camera->GetTransform());
        //	transform->SetLocalPosition({ 0,0,1 });

        //	ObjectPtr<MeshRenderer> meshRenderer = cube->GetComponent<MeshRenderer>();
        //	ObjectPtr < Material> material = meshRenderer->GetMaterial();
        //	material->SetRenderQueue(RenderQueue::Overlay);
        //	material->GetShaders()[0]->SetBlend(Blend::Multiply);
        //	material->SetTexture(0, GameObject::Find(L"太阳")->GetComponent<Light>()->shadowMap.As<Texture>());
        //}
    }
}
