#include "BDXKEngine.h"
#include "Test.h"

class TestMatrixAndInput :public Component {
public:
	Vector2 center = Vector2(Vector2::one) * 100;
	float size = 1;
private:
	void Update()override
	{
		if (Input::GetMouseButton(1))
			center = Input::GetMousePosition();

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

		size += Input::GetMouseScrollDelta().y;
	}

	void OnRenderObject()override
	{
		Matrix matrix = Matrix::identity;
		matrix *= Matrix::Rotate(Time::GetRealtimeSinceStartup() * 100, Vector2::zero);
		matrix *= Matrix::Translate(center);

		Graphics::SetTransform(matrix);
		Graphics::SetBrushColor(Color::blue);

		Graphics::DrawRectangleCenter(Vector2(0, 0), Vector2(200, 70) * size, true);
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