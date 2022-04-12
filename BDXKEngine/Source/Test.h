#pragma once
#include "BDXKEngine.h"


class Test {
public:
	static void TestDebug()
	{
		Debug::Log(true);
		Debug::LogWarning(23.23f);
		Debug::LogError(L"文本");
		Debug::Log((String*)0, 1);
		Debug::Log((String)"微软文档永远的神" + false + 123);
	}

	static void TestWindowBase()
	{
		WindowBase window(L"测试成功");
		TestWindow(window);
	}

	static void TestGameObject()
	{
		BDXKEngine::Awake();

		//在此创建初始场景
		GameObject* gameObject = new GameObject(L"BDXKEngine");
		//组件1
		gameObject->AddComponent<TestComponent>();
		//组件2
		TestComponent* testComponent = gameObject->AddComponent<TestComponent>();
		testComponent->x = 600;


		BDXKEngine::Run();
	}

	static void TestFileOpenDialog()
	{
		Debug::Log(DialogWindow::FileOpenDialog());
	}
private:
	class TestComponent :public Component
	{
	public:
		float x = 300;
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
			Graphics::SetBrushColor(Color::Blue);
			Graphics::DrawRectangle(Vector2(x, 100), Vector2(x + 50, 300), false);
		}
	};

	static void TestWindow(WindowBase& window)
	{
		window.Show();

		//检索消息队列
		MSG msg = {};
		while (GetMessage(&msg, NULL, 0, 0))
		{
			//预处理后交给窗口过程响应
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
};

