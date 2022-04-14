#include "BDXKEngine.h"
#include "Test.h"

class TestComponent :public Component {
public:
	Vector2 center = Vector2(Vector2::one) * 100;
private:
	void Update()override
	{
		if (Input::GetMouseButtonDown(0))
			Debug::Log(L"左键按下", 1);
		if (Input::GetMouseButton(0))
			Debug::Log(L"左键按住", 2);
		if (Input::GetMouseButtonUp(0))
			Debug::Log(L"左键抬起", 5);

		center = Input::mousePosition;
	}

	void OnRenderObject()override
	{
		Matrix matrix = Matrix::identity;
		matrix *= Matrix::Rotate(Time::GetRealtimeSinceStartup() * 100, Vector2::zero);
		matrix *= Matrix::Translate(center);

		Graphics::SetTransform(matrix);
		Graphics::SetBrushColor(Color::blue);

		Graphics::DrawRectangleCenter(Vector2(0,0), Vector2(200, 70), true);
	}
};

int main()
{
	BDXKEngine::Run([]() {
		//在此创建初始场景
		//物体1
		GameObject* gameObject1 = new GameObject(L"物体1");
		gameObject1->AddComponent<TestComponent>();
		////物体2
		//GameObject* gameObject2 = new GameObject(L"物体2");
		//TestComponent* testComponent21 = gameObject2->AddComponent<TestComponent>();
		//testComponent21->x = 200;
		//TestComponent* testComponent22 = gameObject2->AddComponent<TestComponent>();
		//testComponent22->x = 300;
		//testComponent22->rotate = true;
		});
}