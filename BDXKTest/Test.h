#pragma once
#include "../Sources/Runtime/BDXKEngine.h"
#include "MaterialHUD.h"
#include "CreationMenu.h"

namespace Assembly {
	using namespace BDXKEditor;

	void CreateDefaultScene()
	{
		GameObject* camera = CreationMenu::Camera(L"摄像机");
		GameObject* sun = CreationMenu::Light::DirectionalLight(L"太阳");
		GameObject* ground = CreationMenu::Object3D::Plane(L"地面");
		GameObject* sphere = CreationMenu::Object3D::Sphere(L"球");
		//球的展示位置
		{
			Transform* transform = sphere->GetTransform();
			transform->SetLocalPosition({ 0,0.5f,0 });
		}
		//增加材质控制
		{
			MaterialHUD* materialHUD = camera->AddComponent<MaterialHUD>();
			materialHUD->materials.push_back(sphere->GetComponent<MeshRenderer>()->GetMaterial());
			camera->GetTransform()->SetLocalPosition({ 0,1,-10 });
		}
	}

	void Run2()
	{
		CreateDefaultScene();
	}

	void Run1()
	{
		CreateDefaultScene();

		GameObject* aureole = new GameObject(L"光环");
		{
			Transform* transform = aureole->GetTransform();
			transform->SetParent(GameObject::Find(L"摄像机")->GetTransform());
			transform->SetLocalPosition({ 0,0,1 });

			Animator* animator = aureole->AddComponent<Animator>();
			animator->SetAnimation([](Transform* transform)
				{
					Vector3 position = transform->GetLocalPosition();
					position.x = std::cosf(Time::GetRealtimeSinceStartup()) * 0.3f;
					position.y = std::sinf(Time::GetRealtimeSinceStartup()) * 0.3f;
					transform->SetLocalPosition(position);

					Vector3 eulerAngles = transform->GetLocalEulerAngles();
					eulerAngles.z -= 90 * Time::GetDeltaTime();
					transform->SetLocalEulerAngles(eulerAngles);
				});
		}

		GameObject* sphere = CreationMenu::Object3D::Sphere(L"小球");
		{
			Transform* transform = sphere->GetTransform();
			transform->SetParent(aureole->GetTransform());
			transform->SetLocalScale({ 0.1f,0.1f,0.1f });

			Light* light = CreationMenu::Light::PointLight(L"红色点光源")->GetComponent<Light>();
			light->SetLightType(LightType::Point);
			light->SetColor(Color::red);
			light->SetIntensity(0.5f);
			light->GetTransform()->SetParent(sphere->GetTransform());

		}

		GameObject* cube = CreationMenu::Object3D::Cube(L"棍子");
		{
			Transform* transform = cube->GetTransform();
			transform->SetParent(aureole->GetTransform());
			transform->SetLocalScale({ 0.2f,0.05f,0.05f });
		}
	}
}