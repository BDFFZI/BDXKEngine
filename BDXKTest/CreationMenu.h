#pragma once
#include "CameraController.h"
#include "SceneHUD.h"

#define GetResourcesPath(Type,Name) "../Resources/"#Type"/"#Name""
#define GetResourcesPathW(Type,Name) L"../Resources/"#Type"/"#Name""

namespace BDXKEditor {
	using namespace BDXKEngine;

	namespace CreationMenu {
		class Object3D {
		public:
			static GameObject* Cube(const wchar_t* name = L"Cube", Color color = Color::white)
			{
				return CreateObject3D(GetResourcesPath(Meshes, Cube.fbx), name, color);
			}
			static GameObject* Plane(const wchar_t* name = L"Plane", Color color = Color::white)
			{
				Transform* plane = Cube(name, color)->GetTransform();
				plane->SetLocalScale({ 10,0.01f,10 });
				plane->SetLocalPosition({ 0,-0.005f,0 });
				return plane->GetGameObject();
			}
			static GameObject* Sphere(const wchar_t* name = L"Sphere", Color color = Color::white) {
				return CreateObject3D(GetResourcesPath(Meshes, Sphere.fbx), name, color);
			}
		private:
			static GameObject* CreateObject3D(const char* meshPath, const wchar_t* name, Color color = Color::white)
			{
				//加载网格
				StaticMesh meshSource = MeshImporter::ImportFbx((char*)meshPath);
				//创建网格
				Mesh* mesh = new Mesh(meshSource);
				//加载着色器
				Shader* baseShader = new Shader{
					GetResourcesPathW(Shaders,Standard\\VertexShader.hlsl),
					GetResourcesPathW(Shaders,Standard\\PixelShader.hlsl),
					PassType::ForwardBase
				};

				Shader* addShader = new Shader{
					GetResourcesPathW(Shaders,Standard\\VertexShader.hlsl),
					GetResourcesPathW(Shaders,Standard\\PixelShader.hlsl),
					PassType::ForwardAdd,
				};
				addShader->SetBlend(Blend::Additive);

				//创建纹理
				Texture2D* texture2d = new Texture2D{ color };
				//创建材质球
				Material* material = new Material{ {baseShader,addShader} };
				material->SetTexture(0, texture2d);
				//创建物体
				GameObject* gameObject = new GameObject(name);
				MeshRenderer* meshRenderer = gameObject->AddComponent<MeshRenderer>();
				meshRenderer->SetMesh(mesh);
				meshRenderer->SetMaterial(material);

				return gameObject;
			}
		};

		class Light {
		public:
			static GameObject* DirectionalLight(const wchar_t* name = L"DirectionalLight")
			{
				GameObject* light = CreateLight(LightType::Directional, name);
				Transform* transform = light->GetTransform();
				transform->SetLocalEulerAngles({ 45,-45,0 });
				return light;
			}
			static GameObject* PointLight(const wchar_t* name = L"PointLight")
			{
				return CreateLight(LightType::Point, name);
			}
		private:
			static GameObject* CreateLight(LightType lightType, const wchar_t* name = L"Light")
			{
				GameObject* lightObj = new GameObject(name);
				BDXKEngine::Light* light = lightObj->AddComponent<BDXKEngine::Light>();
				light->SetColor(Color::white);
				light->SetIntensity(0.5f);
				light->SetLightType(lightType);
				return lightObj;
			}
		};

		static GameObject* Camera(const wchar_t* name = L"Camera")
		{
			GameObject* camera = new GameObject(name);
			camera->AddComponent<BDXKEngine::Camera>();
			camera->AddComponent<CameraController>();
			camera->AddComponent<SceneHUD>();
			return camera;
		}
	}
}

