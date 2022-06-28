#include "Test.h"
#include "BDXKEngine.h"

class CameraController :public Component, public StartEvent, public UpdateEvent, public DrawGizmosEvent {
	Transform* transform{};

	void OnStart()override
	{
		transform = GetGameObject()->GetTransform();
	}

	void OnUpdate()override
	{
		Vector3 position = transform->GetPosition();
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

			Debug::Log(Input::GetMousePosition().ToString().c_str());
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
		GL2D::DrawCircle(Input::GetMousePosition(), 10, true);
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