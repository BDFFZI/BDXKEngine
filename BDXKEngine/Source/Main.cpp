#include "BDXKEngine.h"
#include "Test.h"

class TestMatrixAndInput :public Component {
private:
	Vector2 center = Vector2(Vector2::one) * 100;
	float size = 1;
	float sleep = 100;


	void Update()override
	{
		if (Input::GetMouseButtonDown(0))
		{
			Debug::Log("锁定", 4);
			Cursor::SetLockState(true);
		}
		if (Input::GetMouseButtonUp(0))
		{
			Debug::Log("解锁", 2);
			Cursor::SetLockState(false);
		}

		if (Input::GetMouseButton(1))
			center = Input::GetMousePosition();

		float move = Time::GetDeltaTime() * 100;
		if (Input::GetKey(KeyCode::Shift))
		{
			Debug::Log(L"加速");
			move *= 3;
		}

		if (Input::GetKeyDown(KeyCode::Shift))sleep *= 10;
		if (Input::GetKeyUp(KeyCode::Shift))sleep /= 10;

		if (Input::GetKey(KeyCode::W))
			center.y -= move;
		if (Input::GetKey(KeyCode::S))
			center.y += move;
		if (Input::GetKey(KeyCode::A))
			center.x -= move;
		if (Input::GetKey(KeyCode::D))
			center.x += move;

		size += Input::GetMouseScrollDelta().y;
	}

	void OnRenderObject()override
	{
		Matrix3x2 matrix = Matrix3x2::identity;
		matrix *= Matrix3x2::Scale((Vector2)Vector2::one * size);
		matrix *= Matrix3x2::Rotate(Time::GetRealtimeSinceStartup() * sleep, Vector2::zero);
		matrix *= Matrix3x2::Translate(center);

		Graphics::SetTransform(matrix);
		Graphics::SetBrushColor(Color::blue);

		Graphics::DrawRectangleCenter(Vector2(0, 0), Vector2(100, 50), true);
	}
};

int main()
{
	BDXKEngine::Run([]() {
		//在此创建初始场景
		//物体1
		GameObject* gameObject1 = new GameObject(L"物体1");
		gameObject1->AddComponent<TestMatrixAndInput>();
		////物体2
		//GameObject* gameObject2 = new GameObject(L"物体2");
		//TestObjectAndGraphics* testComponent21 = gameObject2->AddComponent<TestObjectAndGraphics>();
		//testComponent21->x = 200;
		//TestObjectAndGraphics* testComponent22 = gameObject2->AddComponent<TestObjectAndGraphics>();
		//testComponent22->x = 300;
		//testComponent22->rotate = true;
		});
}