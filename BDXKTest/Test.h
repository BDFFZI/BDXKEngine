#pragma once
#include "../Sources/Runtime/BDXKEngine.h"
#include "MaterialHUD.h"
#include "CreationMenu.h"

namespace Assembly {
	using namespace BDXKEditor;

	void Run2()
	{
		CreationMenu::Camera();
		CreationMenu::Object3D::Plane(L"Ground");
	}

	void Run1()
	{
		//加载网格
		StaticMesh cubeMesh = MeshImporter::ImportFbx((char*)GetResourcesPath(Meshes, Cube.fbx));
		StaticMesh sphereMesh = MeshImporter::ImportFbx((char*)GetResourcesPath(Meshes, Sphere.fbx));
		//创建网格
		Mesh* cube = new Mesh(cubeMesh);
		Mesh* sphere = new Mesh(sphereMesh);
		//加载着色器
		Shader* baseShader = new Shader{
			(char*)GetResourcesPath(Shaders,VertexShader.cso),
			(char*)GetResourcesPath(Shaders,PixelShader.cso),
			PassType::ForwardBase,
			true
		};
		Shader* addShader = new Shader{
			(char*)GetResourcesPath(Shaders,VertexShader.cso),
			(char*)GetResourcesPath(Shaders,PixelShader.cso),
			PassType::ForwardAdd,
			false
		};
		//创建纹理
		Texture2D* texture2dWhite = new Texture2D{ Color::white };
		Texture2D* texture2dRed = new Texture2D{ Color::red };
		//创建材质球
		Material* materialWhite = new Material{ {baseShader,addShader} };
		materialWhite->SetTexture(0, texture2dWhite);
		Material* materialRed = new Material{ {baseShader,addShader} };
		materialRed->SetTexture(0, texture2dRed);

		//添加场景物体
		GameObject* camera = new GameObject(L"Camera");
		{
			camera->AddComponent<Camera>();
			camera->AddComponent<CameraController>();
			camera->AddComponent<SceneHUD>();
			MaterialHUD* hud = camera->AddComponent<MaterialHUD>();
			hud->materials.push_back(materialWhite);
			hud->materials.push_back(materialRed);

			camera->GetTransform()->SetLocalPosition({ 0,0,-13 });
		}

		GameObject* lightObj = new GameObject(L"Light");
		{
			Light* light = lightObj->AddComponent<Light>();
			light->SetColor(Color::white);
			light->SetIntensity(0.5f);
			lightObj->GetTransform()->SetLocalEulerAngles({ 45,-45,0 });
		}

		GameObject* aureole = new GameObject(L"Aureole");
		{
			MeshRenderer* meshRenderer = aureole->AddComponent<MeshRenderer>();
			meshRenderer->SetMesh(sphere);
			meshRenderer->SetMaterial(materialRed);
			Transform* transform = aureole->GetTransform();
			transform->SetParent(camera->GetTransform());
			transform->SetLocalPosition({ 0,0,2 });
			transform->SetLocalScale({ 0.1f,0.1f,0.1f });
			Animator* animator = aureole->AddComponent<Animator>();
			animator->SetAnimation([](Transform* transform)
				{
					Vector3 position = transform->GetLocalPosition();
					position.x = std::cosf(Time::GetRealtimeSinceStartup()) * 0.5f;
					position.y = std::sinf(Time::GetRealtimeSinceStartup()) * 0.5f;
					transform->SetLocalPosition(position);

					Vector3 eulerAngles = transform->GetLocalEulerAngles();
					eulerAngles.z -= 90 * Time::GetDeltaTime();
					transform->SetLocalEulerAngles(eulerAngles);
				});

			Light* light = aureole->AddComponent<Light>();
			light->SetLightType(LightType::Point);
			light->SetColor(Color::red);
			light->SetIntensity(0.5f);
		}

		GameObject* ground = new GameObject(L"Ground");
		{
			MeshRenderer* meshRenderer = ground->AddComponent<MeshRenderer>();
			meshRenderer->SetMesh(cube);
			meshRenderer->SetMaterial(materialWhite);
			Transform* transform = ground->GetTransform();
			transform->SetLocalPosition({ 0,-0.5f,10 });
			transform->SetLocalScale({ 10,0.01f,10 });
		}

		GameObject* sphereShow = new GameObject(L"Sphere");
		{
			MeshRenderer* meshRenderer = sphereShow->AddComponent<MeshRenderer>();
			meshRenderer->SetMesh(sphere);
			meshRenderer->SetMaterial(materialWhite);
			Transform* transform = sphereShow->GetTransform();
			transform->SetLocalPosition({ 0,0,-10 });
			transform->SetLocalScale({ 1.5f,1.5f,1.5f });
		}

		GameObject* center = new GameObject(L"Center");
		{
			MeshRenderer* meshRenderer = center->AddComponent<MeshRenderer>();
			meshRenderer->SetMesh(sphere);
			meshRenderer->SetMaterial(materialWhite);
			Transform* transform = center->GetTransform();
			transform->SetLocalPosition({ 0,0,10 });
			Animator* animator = center->AddComponent<Animator>();
			animator->SetAnimation([](Transform* transform)
				{
					transform->SetLocalScale((Vector3)Vector3::one * std::fabsf(std::cosf(Time::GetRealtimeSinceStartup())));
					Vector3 position = transform->GetLocalPosition();
					position.y = std::cosf(Time::GetRealtimeSinceStartup());
					transform->SetLocalPosition(position);
				});
		}

		GameObject* box = new GameObject(L"Box");
		{
			MeshRenderer* meshRenderer = box->AddComponent<MeshRenderer>();
			meshRenderer->SetMesh(cube);
			meshRenderer->SetMaterial(materialWhite);
			Transform* transform = box->GetTransform();
			transform->SetParent(center->GetTransform());
			transform->SetLocalPosition({ 0,0,0 });
			Animator* animator = box->AddComponent<Animator>();
			animator->SetAnimation([](Transform* transform)
				{
					Vector3 position = transform->GetLocalPosition();
					position.x = std::cosf(Time::GetRealtimeSinceStartup() * 5) * 5;
					position.z = std::sinf(Time::GetRealtimeSinceStartup() * 5) * 5;
					transform->SetLocalPosition(position);

					Vector3 eulerAngles = transform->GetLocalEulerAngles();
					eulerAngles.x = Time::GetRealtimeSinceStartup() * 20;
					eulerAngles.y = Time::GetRealtimeSinceStartup() * 20;
					eulerAngles.z = Time::GetRealtimeSinceStartup() * 20;
					transform->SetLocalEulerAngles(eulerAngles);
				});
		}
	}
}