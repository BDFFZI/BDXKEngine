#include "CameraController.h"

#define GetResourcesPath(Type,Name) "C:/Users/BDFFZI/Desktop/BDXKEngine/Resources/"#Type"/"#Name""

class MaterialHUD :public Component, public DrawGizmosEvent {
public:
	Material* material;
	Material* material2;
	float metallic = 0.9f;
	float smoothness = 0.5f;

private:
	void OnDrawGizmos()override {
		Vector2 windowSize = Screen::GetSize();
		Rect rect = {
			windowSize.x - 100,
			windowSize.y - 90,
			90,
			20
		};


		smoothness = GUI::Slider(rect, smoothness, 0, 1);
		rect.y -= 30;
		metallic = GUI::Slider(rect, metallic, 0, 1);

		material->SetFloat(0, metallic);
		material->SetFloat(1, smoothness);
		material2->SetFloat(0, metallic);
		material2->SetFloat(1, smoothness);
	}
};

int main()
{
	BDXKEngine::Run([&]() {
		//从磁盘外加载资源
		StaticMesh cubeMesh = MeshImporter::ImportFbx((char*)GetResourcesPath(Meshes, Cube.fbx));
		StaticMesh sphereMesh = MeshImporter::ImportFbx((char*)GetResourcesPath(Meshes, Sphere.fbx));
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

		//创建一个白色的纹理 TODO 这里有BUG，使用后会导致GL2D无法调整渲染目标大小
		Texture2D* texture2d = new Texture2D{ 1024,1024 };
		Graphics::SetRenderTarget(texture2d);
		GL::Begin(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		GL::Clear(true, true, Color::white);
		GL::End();
		Graphics::SetRenderTarget(nullptr);
		//红色的纹理
		Texture2D* texture2dred = new Texture2D{ 1024,1024 };
		Graphics::SetRenderTarget(texture2dred);
		GL::Begin(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		GL::Clear(true, true, Color::red);
		GL::End();
		Graphics::SetRenderTarget(nullptr);

		//转为渲染器资源
		Mesh* cube = new Mesh(cubeMesh);
		Mesh* sphere = new Mesh(sphereMesh);
		Material* material = new Material{ {baseShader,addShader} };
		material->SetTexture(0, texture2d);
		Material* materialred = new Material{ {baseShader,addShader} };
		materialred->SetTexture(0, texture2dred);

		//添加场景物体
		GameObject* lightObj = new GameObject(L"Light");
		{
			Light* light = lightObj->AddComponent<Light>();
			light->SetColor(Color::white);
			light->SetIntensity(0.5f);
			lightObj->GetTransform()->SetLocalEulerAngles({ 45,-45,0 });
		}

		GameObject* camera = new GameObject(L"Camera");
		{
			camera->AddComponent<Camera>();
			camera->AddComponent<CameraController>();
			MaterialHUD* hud = camera->AddComponent<MaterialHUD>();
			hud->material = material;
			hud->material2 = materialred;
			camera->GetTransform()->SetLocalPosition({ 0,0,-13 });
		}

		GameObject* aureole = new GameObject(L"Aureole");
		{
			MeshRenderer* meshRenderer = aureole->AddComponent<MeshRenderer>();
			meshRenderer->SetMesh(sphere);
			meshRenderer->SetMaterial(materialred);
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
			meshRenderer->SetMaterial(material);
			Transform* transform = ground->GetTransform();
			transform->SetLocalPosition({ 0,-0.5f,10 });
			transform->SetLocalScale({ 10,0.01f,10 });
		}

		GameObject* sphereShow = new GameObject(L"Sphere");
		{
			MeshRenderer* meshRenderer = sphereShow->AddComponent<MeshRenderer>();
			meshRenderer->SetMesh(sphere);
			meshRenderer->SetMaterial(material);
			Transform* transform = sphereShow->GetTransform();
			transform->SetLocalPosition({ 0,0,-10 });
			transform->SetLocalScale({ 1.5f,1.5f,1.5f });
		}

		GameObject* center = new GameObject(L"Center");
		{
			MeshRenderer* meshRenderer = center->AddComponent<MeshRenderer>();
			meshRenderer->SetMesh(sphere);
			meshRenderer->SetMaterial(material);
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
			meshRenderer->SetMaterial(material);
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
		});
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
	main();
}
