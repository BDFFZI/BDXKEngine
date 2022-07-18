#pragma once
#include "../Sources/Runtime/BDXKEngine.h"
#include "MaterialHUD.h"
#include "CreationMenu.h"
#include "CreateCube.h"

namespace Assembly {
	using namespace BDXKEditor;

	void CreateDefaultScene()
	{
		ObjectPtr<GameObject> camera = CreationMenu::Camera(L"�����");
		ObjectPtr<GameObject> sun = CreationMenu::Light::DirectionalLight(L"̫��");
		ObjectPtr<GameObject> ground = CreationMenu::Object3D::Plane(L"����");
		ObjectPtr<GameObject> sphere = CreationMenu::Object3D::Sphere(L"��");
		//���չʾλ��
		{
			ObjectPtr<Transform> transform = sphere->GetTransform();
			transform->SetLocalPosition({ 0,0.5f,0 });
		}
		//���Ӳ��ʿ���
		{
			ObjectPtr<MaterialHUD> materialHUD = camera->AddComponent<MaterialHUD>();
			materialHUD->materials.push_back(sphere->GetComponent<MeshRenderer>()->GetMaterial());
			camera->GetTransform()->SetLocalPosition({ 0,1,-10 });
		}
	}
	void TestTransparency()
	{
		CreateDefaultScene();


	}
	void TestLight()
	{
		CreateDefaultScene();

		ObjectPtr<GameObject> aureole = new GameObject(L"�⻷");
		{
			ObjectPtr<Transform> transform = aureole->GetTransform();
			transform->SetParent(GameObject::Find(L"�����")->GetTransform());
			transform->SetLocalPosition({ 0,0,1 });

			ObjectPtr<Animator> animator = aureole->AddComponent<Animator>();
			animator->SetAnimation([](ObjectPtr<Transform> transform)
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

		ObjectPtr<GameObject> sphere = CreationMenu::Object3D::Sphere(L"С��");
		{
			ObjectPtr<Transform> transform = sphere->GetTransform();
			transform->SetParent(aureole->GetTransform());
			transform->SetLocalScale({ 0.1f,0.1f,0.1f });

			ObjectPtr<Light> light = CreationMenu::Light::PointLight(L"��ɫ���Դ")->GetComponent<Light>();
			light->SetLightType(LightType::Point);
			light->SetColor(Color::red);
			light->SetIntensity(0.5f);
			light->GetTransform()->SetParent(sphere->GetTransform());

		}

		ObjectPtr<GameObject> cube = CreationMenu::Object3D::Cube(L"����");
		{
			ObjectPtr<Transform> transform = cube->GetTransform();
			transform->SetParent(aureole->GetTransform());
			transform->SetLocalScale({ 0.2f,0.05f,0.05f });
		}
	}
}