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

			//Debug::Log(Input::GetMousePosition().ToString().c_str());
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
		GL2D::DrawRectangle({ 10,10 }, { 180,30 }, true);
	}
};


int main()
{
	BDXKEngine::Run([&]() {


		GameObject* camera = new GameObject(L"Camera");
		{
			camera->AddComponent<Camera>();
			camera->AddComponent<CameraController>();
		}

		GameObject* aureole = new GameObject(L"Aureole");
		{
			Mesh* mesh = new Mesh{};
			Shader* shader = new Shader{};
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
			Mesh* mesh = new Mesh{};
			Shader* shader = new Shader{};
			MeshRenderer* meshRenderer = ground->AddComponent<MeshRenderer>();
			meshRenderer->SetMesh(mesh);
			meshRenderer->SetShader(shader);
			Transform* transform = ground->GetTransform();
			transform->SetLocalPosition({ 0,-0.5f,10 });
			transform->SetLocalScale({ 10,0.1f,10 });
		}

		GameObject* center = new GameObject(L"Center");
		{
			Mesh* mesh = new Mesh{};
			Shader* shader = new Shader{};
			MeshRenderer* meshRenderer = center->AddComponent<MeshRenderer>();
			meshRenderer->SetMesh(mesh);
			meshRenderer->SetShader(shader);
			Transform* transform = center->GetTransform();
			//transform->SetParent(camera->GetTransform());
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
			Mesh* mesh = new Mesh{};
			Shader* shader = new Shader{};
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

}