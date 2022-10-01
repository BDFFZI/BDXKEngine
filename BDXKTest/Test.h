// #pragma once
// #include <BDXKEngine/Engine.h>
// #include <BDXKEditor/Menu/CreationMenu.h>
// #include "CreateCube.h"
//
// namespace Assembly
// {
//     using namespace BDXKEditor;
//
//     inline void TestTransparency()
//     {
//         const ObjectPtr<Camera> camera = GameObject::Find("摄像机")->GetComponent<Camera>();
//         camera->GetGameObject()->AddComponent<CreateCube>();
//     }
//
//     void TestLight()
//     {
//         ObjectPtr<GameObject> aureole = GameObject::Create("光环");
//         {
//             ObjectPtr<Transform> transform = aureole->GetTransform();
//             transform->SetParent(GameObject::Find("摄像机")->GetTransform());
//             transform->SetLocalPosition({0, 0, 0.7f});
//
//             ObjectPtr<Animator> animator = aureole->AddComponent<Animator>();
//             animator->SetAnimation(AnimatorClip::Create([](ObjectPtr<Transform> transform)
//             {
//                 Vector3 position = transform->GetLocalPosition();
//                 position.x = std::cosf(Time::GetRealtimeSinceStartup()) * 0.3f;
//                 position.y = std::sinf(Time::GetRealtimeSinceStartup()) * 0.3f;
//                 transform->SetLocalPosition(position);
//
//                 Vector3 eulerAngles = transform->GetLocalEulerAngles();
//                 eulerAngles.z -= 90 * Time::GetDeltaTime();
//                 transform->SetLocalEulerAngles(eulerAngles);
//             }));
//         }
//
//         ObjectPtr<GameObject> sphere = CreationMenu::Object3D::Sphere("小球");
//         ObjectPtr<Light> sphere_light;
//         {
//             ObjectPtr<Transform> transform = sphere->GetTransform();
//             transform->SetParent(aureole->GetTransform());
//             transform->SetLocalScale({0.1f, 0.1f, 0.1f});
//
//             sphere_light = CreationMenu::Light::PointLight("红色点光源")->GetComponent<Light>();
//             sphere_light->SetLightType(LightType::Point);
//             sphere_light->SetColor(Color::red);
//             sphere_light->SetIntensity(0.5f);
//             sphere_light->GetTransform()->SetParent(sphere->GetTransform());
//         }
//
//         ObjectPtr<GameObject> cube = CreationMenu::Object3D::Cube("棍子");
//         {
//             ObjectPtr<Transform> transform = cube->GetTransform();
//             transform->SetParent(aureole->GetTransform());
//             transform->SetLocalScale({0.2f, 0.05f, 0.05f});
//         }
//     }
//
//     void TestShadow()
//     {
//         ObjectPtr<GameObject> camera = GameObject::Find("摄像机");
//         ObjectPtr<Light> pointlight = GameObject::Find("红色点光源")->GetComponent<Light>();
//         //GraphicsSettings::skybox = pointlight->shadowMapCube;
//
//         //ObjectPtr<GameObject> cube = CreationMenu::Object3D::Cube("阴影贴图显示器");
//         ////阴影贴图显示器
//         //{
//         //	ObjectPtr<Transform> transform = cube->GetTransform();
//         //	transform->SetParent(camera->GetTransform());
//         //	transform->SetLocalPosition({ 0,0,1 });
//
//         //	ObjectPtr<MeshRenderer> meshRenderer = cube->GetComponent<MeshRenderer>();
//         //	ObjectPtr < Material> material = meshRenderer->GetMaterial();
//         //	material->SetRenderQueue(RenderQueue::Overlay);
//         //	material->GetShaders()[0]->SetBlend(Blend::Multiply);
//         //	material->SetTexture(0, GameObject::Find("太阳")->GetComponent<Light>()->shadowMap.As<Texture>());
//         //}
//     }
// }
