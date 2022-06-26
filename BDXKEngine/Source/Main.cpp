#include "Test.h"
#include "BDXKEngine.h"

class CameraController :public Component, public StartEvent, public UpdateEvent {
	Transform* transform{};

	void OnStart()override
	{
		transform = GetGameObject()->GetTransform();
	}

	void OnUpdate()override
	{
		Vector3 localPosition = transform->GetLocalPosition();
		float deltaTime = Time::GetDeltaTime();

		if (Input::GetKey(KeyCode::W))
		{
			localPosition.y += deltaTime * 3;
		}
		if (Input::GetKey(KeyCode::S))
		{
			localPosition.y -= deltaTime * 3;
		}
		if (Input::GetKey(KeyCode::A))
		{
			localPosition.x -= deltaTime * 3;
		}
		if (Input::GetKey(KeyCode::D))
		{
			localPosition.x += deltaTime * 3;
		}
		localPosition.z += Input::GetMouseScrollDelta();
		transform->SetLocalPosition(localPosition);

		Vector3 localEulerAngles = transform->GetLocalEulerAngles();
		if (Input::GetKey(KeyCode::I))
		{
			localEulerAngles.x -= deltaTime * 20;
		}
		if (Input::GetKey(KeyCode::K))
		{
			localEulerAngles.x += deltaTime * 20;
		}
		if (Input::GetKey(KeyCode::J))
		{
			localEulerAngles.y -= deltaTime * 20;
		}
		if (Input::GetKey(KeyCode::L))
		{
			localEulerAngles.y += deltaTime * 20;
		}
		transform->SetLocalEulerAngles(localEulerAngles);

		Debug::Log(localEulerAngles.ToString().c_str());
	}
};


int main()
{
	BDXKEngine::Run([&]() {
		Mesh* mesh = new Mesh{};
		Shader* shader = new Shader{};

		GameObject* camera = new GameObject(L"Camera");
		camera->AddComponent<Camera>();
		camera->AddComponent<CameraController>();

		GameObject* renderer = new GameObject(L"renderer");
		MeshRenderer* meshRenderer = renderer->AddComponent<MeshRenderer>();
		meshRenderer->SetMesh(mesh);
		meshRenderer->SetShader(shader);
		Transform* transform = renderer->GetTransform();
		transform->SetLocalPosition({ 0,0,10 });
		});
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{

}