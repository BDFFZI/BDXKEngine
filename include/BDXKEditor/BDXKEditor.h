#pragma once
#include <BDXKEngine/Engine.h>
#include <rapidjson/writer.h>
#include "EditorSystem.h"
#include "BDXKEngine/Base/Serialization/Json/JsonWriter.h"
#include "EditorWindow/CreationView.h"


namespace BDXKEditor
{
    using namespace BDXKEngine;

    inline void TestLight()
    {
        ObjectPtr<GameObject> aureole = EditorSystem::GetMainScene()->AddGameObject("光环");
        {
            ObjectPtr<Transform> transform = aureole->GetTransform();
            transform->SetParent(EditorSystem::GetMainScene()->Find("摄像机")->GetTransform());
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

        ObjectPtr<GameObject> sphere = CreationWindow::Object3D::Sphere("小球");
        ObjectPtr<Light> sphere_light;
        {
            ObjectPtr<Transform> transform = sphere->GetTransform();
            transform->SetParent(aureole->GetTransform());
            transform->SetLocalScale({0.1f, 0.1f, 0.1f});

            sphere_light = CreationWindow::Light::PointLight("红色点光源")->GetComponent<Light>();
            sphere_light->SetLightType(LightType::Point);
            sphere_light->SetColor(Color::red);
            sphere_light->SetIntensity(0.5f);
            sphere_light->GetTransform()->SetParent(sphere->GetTransform());
        }

        ObjectPtr<GameObject> cube = CreationWindow::Object3D::Cube("棍子");
        {
            ObjectPtr<Transform> transform = cube->GetTransform();
            transform->SetParent(aureole->GetTransform());
            transform->SetLocalScale({0.2f, 0.05f, 0.05f});
        }
    }
    inline void RunEditor()
    {
        Engine::Run([](const ObjectPtr<Scene>& scene)
        {
            EditorSystem::SetMainScene(scene);

            CreationWindow::CreateDefaultScene();
            TestLight();
        });

        Debug::Log("\n按任意键退出");
        std::getchar();
    }
}
