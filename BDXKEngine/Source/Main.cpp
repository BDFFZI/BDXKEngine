#include "BDXKEngine.h"
#include "Test.h"

class TestComponent :public Component
{
public:
	float x = 100;
	bool rotate = false;

private:
	float y = 0;

	void OnAwake()override {
		Debug::Log(L"我出生了");
		Debug::LogError((String)L"我的名字是" + GetGameObject()->name);
	}

	void OnStart()override {
		GameObject* gameObject = GetGameObject();
		Debug::LogWarning(String("我已经准备好在横坐标") + x + "处画图");
	}

	void OnUpdate()override {
		y = Math::Sin(Time::GetRealtimeSinceStartup()) * 100;
	}

	void OnRenderObject() override {
		Graphics::SetBrushColor({ 0, 1, 0, 0.5f });
		Graphics::DrawCircle({ x,200 + y }, 100, true);
		Graphics::SetBrushColor({ 1, 0, 0, 0.5f });
		Graphics::DrawCircle({ x + 50,200 + (1 - y) }, 100, true);
	}

	void OnDrawGizmos() override
	{
		if(rotate)Graphics::SetTransform(Matrix::Rotate(Time::GetRealtimeSinceStartup()*10, Vector2(x+25, 200)));
		Graphics::SetBrushColor(Color::Blue);
		Graphics::DrawRectangle(Vector2(x, 100), Vector2(x + 50, 300), false);
		Graphics::SetTransform();
	}
};

int main()
{
	BDXKEngine::Run([]() {
		//在此创建初始场景
		//物体1
		GameObject* gameObject1 = new GameObject(L"物体1");
		gameObject1->AddComponent<TestComponent>();
		//物体2
		GameObject* gameObject2 = new GameObject(L"物体2");
		TestComponent* testComponent21 = gameObject2->AddComponent<TestComponent>();
		testComponent21->x = 200;
		TestComponent* testComponent22 = gameObject2->AddComponent<TestComponent>();
		testComponent22->x = 300;
		testComponent22->rotate = true;
		});
}