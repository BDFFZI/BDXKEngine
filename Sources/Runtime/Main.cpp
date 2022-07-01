#include "Test.h"
#include "BDXKEngine.h"
#include<fbxsdk.h>

class CameraController :public Component, public StartEvent, public UpdateEvent, public DrawGizmosEvent {
	Transform* transform{};
	std::wstring text{ L"我是一个输入框。" };
	void OnStart()override
	{
		transform = GetGameObject()->GetTransform();
	}

	void OnUpdate()override
	{
		Vector3 position = transform->GetLocalPosition();
		float deltaTime = Time::GetDeltaTime();

		if (Input::GetKey(KeyCode::W))
		{
			position += transform->GetFront() * deltaTime * 4;
		}
		if (Input::GetKey(KeyCode::S))
		{
			position += -transform->GetFront() * deltaTime * 4;
		}
		if (Input::GetKey(KeyCode::A))
		{
			position += -transform->GetRight() * deltaTime * 4;
		}
		if (Input::GetKey(KeyCode::D))
		{
			position += transform->GetRight() * deltaTime * 4;
		}
		if (Input::GetKey(KeyCode::Q))
		{
			position += -transform->GetUp() * deltaTime * 4;
		}
		if (Input::GetKey(KeyCode::E))
		{
			position += transform->GetUp() * deltaTime * 4;
		}

		transform->SetLocalPosition(position);

		if (Input::GetMouseButton(1))
		{
			Vector3 localEulerAngles = transform->GetLocalEulerAngles();
			Vector2 mouseMoveDelta = Input::GetMouseMoveDelta();

			localEulerAngles.y += mouseMoveDelta.x * deltaTime * 5;
			localEulerAngles.x += mouseMoveDelta.y * deltaTime * 5;

			if (Input::GetKey(KeyCode::Z))
			{
				localEulerAngles.z += deltaTime * 20;
			}
			if (Input::GetKey(KeyCode::X))
			{
				localEulerAngles.z -= deltaTime * 20;
			}

			transform->SetLocalEulerAngles(localEulerAngles);

		}

		if (Input::GetMouseButtonDown(1))
		{
			Cursor::SetLockState(true);
			Cursor::SetVisible(false);
		}
		if (Input::GetMouseButtonUp(1))
		{
			Cursor::SetLockState(false);
			Cursor::SetVisible(true);
		}
	}

	void OnDrawGizmos()override {
		Vector2 size = Screen::GetSize();
		if (GUI::Button({ 10,size.y - 40,180,30 }, L"按钮"))
		{
			Debug::Log(L"Hello BDXKEngine");
		}
		GUI::TextArea({ 200,size.y - 40,200,30 }, L"鼠标位置:" + Input::GetMousePosition().ToString());
		text = GUI::TextArea({ 410,size.y - 40,size.x - 420,30 }, text);
	}
};

#define GetResourcesPath(Type,Name) "C:/Users/BDFFZI/Desktop/BDXKEngine/Resources/"#Type"/"#Name"";

int main()
{
	const char* fileName = GetResourcesPath(Meshes,Cube.fbx);
	Debug::Log(fileName);
	//初始化FbxManager环境
	FbxManager* manager = FbxManager::Create();
	FbxIOSettings* ioSettings = FbxIOSettings::Create(manager, "FbxIOSettings");
	manager->SetIOSettings(ioSettings);
	//创建导入器
	FbxImporter* importer = FbxImporter::Create(manager, "FbxImporter");
	assert(importer->Initialize(fileName));
	//导入Fbx数据
	FbxScene* scene = FbxScene::Create(manager, "FbxScene");
	importer->Import(scene);
	importer->Destroy();
	//解析Fbx数据
	FbxNode* node = scene->GetRootNode();
	if (node != nullptr)
	{
		FbxNode* child = node->GetChild(0);
		Debug::Log(child->GetName());
		FbxMesh* mesh = child->GetMesh();
		fbxsdk::FbxVector4* vertices = mesh->GetControlPoints();
		int vertexCount = mesh->GetControlPointsCount();

		for (int i = 0; i < vertexCount; i++)
		{
			fbxsdk::FbxVector4 vector4 = *(vertices + i);
			//Debug::Log((String)mesh-> mesh->GetPolygonVertex(i / 3, i % 3));
			std::wstringstream stream;
			stream << "(" << vector4[0] << "," << vector4[1] << "," << vector4[2] << "," << vector4[3] << ")";

			Debug::Log(stream.str());
		}

		mesh->Destroy();
	}

	node->Destroy();
	scene->Destroy();
	ioSettings->Destroy();
	manager->Destroy();

	return 0;

	BDXKEngine::Run([&]() {
		Mesh* mesh = new Mesh{};
		Shader* shader = new Shader{};

		GameObject* camera = new GameObject(L"Camera");
		{
			camera->AddComponent<Camera>();
			camera->AddComponent<CameraController>();
		}

		GameObject* aureole = new GameObject(L"Aureole");
		{
			MeshRenderer* meshRenderer = aureole->AddComponent<MeshRenderer>();
			meshRenderer->SetMesh(mesh);
			meshRenderer->SetShader(shader);
			Transform* transform = aureole->GetTransform();
			transform->SetParent(camera->GetTransform());
			transform->SetLocalPosition({ 0,0,3 });
			transform->SetLocalScale({ 0.1f,0.1f,1 });
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
		}

		GameObject* ground = new GameObject(L"Ground");
		{
			MeshRenderer* meshRenderer = ground->AddComponent<MeshRenderer>();
			meshRenderer->SetMesh(mesh);
			meshRenderer->SetShader(shader);
			Transform* transform = ground->GetTransform();
			transform->SetLocalPosition({ 0,-0.5f,10 });
			transform->SetLocalScale({ 10,0.1f,10 });
		}

		GameObject* center = new GameObject(L"Center");
		{
			MeshRenderer* meshRenderer = center->AddComponent<MeshRenderer>();
			meshRenderer->SetMesh(mesh);
			meshRenderer->SetShader(shader);
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
			meshRenderer->SetMesh(mesh);
			meshRenderer->SetShader(shader);
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