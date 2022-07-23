#pragma once
#include "CameraController.h"
#include "SceneHUD.h"

namespace BDXKEditor {
	using namespace BDXKEngine;

	namespace CreationMenu {
		class Object3D {
		public:
			static ObjectPtr<GameObject> Cube(const wchar_t* name = L"Cube", Color color = Color::white)
			{
				return CreateObject3D(GetResourcesPath(Meshes, Cube.fbx), name, color);
			}
			static ObjectPtr<GameObject> Plane(const wchar_t* name = L"Plane", Color color = Color::white)
			{
				ObjectPtr<Transform> plane = Cube(name, color)->GetTransform();
				plane->SetLocalScale({ 10,0.01f,10 });
				plane->SetLocalPosition({ 0,-0.005f,0 });
				return plane->GetGameObject();
			}
			static ObjectPtr<GameObject> Sphere(const wchar_t* name = L"Sphere", Color color = Color::white) {
				return CreateObject3D(GetResourcesPath(Meshes, Sphere.fbx), name, color);
			}
		private:
			static ObjectPtr<GameObject> CreateObject3D(const char* meshPath, const wchar_t* name, Color color = Color::white)
			{
				//加载网格
				StaticMesh meshSource = MeshImporter::ImportFbx((char*)meshPath);
				ObjectPtr<Mesh> mesh = meshSource.CreateMesh();
				//加载着色器
				ObjectPtr<Shader> baseShader = new Shader{
					GetResourcesPathW(Shaders,Standard\\VertexShader.hlsl),
					GetResourcesPathW(Shaders,Standard\\PixelShader.hlsl),
					PassType::ForwardBase
				};
				ObjectPtr<Shader> addShader = new Shader{
					GetResourcesPathW(Shaders,Standard\\VertexShader.hlsl),
					GetResourcesPathW(Shaders,Standard\\PixelShader.hlsl),
					PassType::ForwardAdd,
				};
				addShader->SetBlend(Blend::Additive);

				//创建纹理
				ObjectPtr<Texture2D> texture2d = new Texture2D{ color };
				//创建材质球
				ObjectPtr<Material> material = new Material{ {baseShader,addShader} };
				material->SetTexture(0, texture2d.As<Texture>());
				//创建物体
				ObjectPtr<GameObject> gameObject = { new GameObject(name) };
				ObjectPtr<MeshRenderer> meshRenderer = gameObject->AddComponent<MeshRenderer>();
				meshRenderer->SetMesh(mesh);
				meshRenderer->SetMaterial(material);

				return gameObject;
			}
		};

		class Light {
		public:
			static ObjectPtr<GameObject> DirectionalLight(const wchar_t* name = L"DirectionalLight")
			{
				ObjectPtr<GameObject> light = CreateLight(LightType::Directional, name);
				ObjectPtr<Transform> transform = light->GetTransform();
				transform->SetLocalEulerAngles({ 45,-45,0 });
				return light;
			}
			static ObjectPtr<GameObject> PointLight(const wchar_t* name = L"PointLight")
			{
				return CreateLight(LightType::Point, name);
			}
		private:
			static ObjectPtr<GameObject> CreateLight(LightType lightType, const wchar_t* name = L"Light")
			{
				ObjectPtr<GameObject> lightObj = new GameObject(name);
				ObjectPtr<BDXKEngine::Light> light = lightObj->AddComponent<BDXKEngine::Light>();
				light->SetColor(Color::white);
				light->SetIntensity(0.5f);
				light->SetLightType(lightType);
				return lightObj;
			}
		};

		static ObjectPtr<GameObject> Camera(const wchar_t* name = L"Camera")
		{
			ObjectPtr<GameObject> camera = new GameObject(name);
			camera->AddComponent<BDXKEngine::Camera>();
			camera->AddComponent<CameraController>();
			camera->AddComponent<SceneHUD>();
			return camera;
		}
	}
}

