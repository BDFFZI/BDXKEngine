#pragma once
#include "BDXKEngine.h"


class Test {
public:
	class TestComponent :public Component
	{
	public:
		float x = 100;
		bool rotate = false;

	private:
		float y = 0;

		void Awake()override {
			Debug::Log(L"我出生了");
			Debug::LogError((String)L"我的名字是" + GetGameObject()->name);
		}

		void OnStart()override {
			GameObject* gameObject = GetGameObject();
			Debug::LogWarning(String("我已经准备好在横坐标") + x + "处画图");
		}

		void Update()override {
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
			if (rotate)Graphics::SetTransform(Matrix::Rotate(Time::GetRealtimeSinceStartup() * 10, Vector2(x + 25, 200)));
			Graphics::SetBrushColor(Color::blue);
			Graphics::DrawRectangle(Vector2(x, 100), Vector2(x + 50, 300), false);
			Graphics::SetTransform();
		}
	};

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

	static void TestFileOpenDialog()
	{
		Debug::Log(WindowUtility::GetOpenFilePath());
	}

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

