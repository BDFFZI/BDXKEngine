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
			Debug::Log(L"�ҳ�����");
			Debug::LogError((String)L"�ҵ�������" + GetGameObject()->name);
		}

		void OnStart()override {
			GameObject* gameObject = GetGameObject();
			Debug::LogWarning(String("���Ѿ�׼�����ں�����") + x + "����ͼ");
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
		Debug::LogError(L"�ı�");
		Debug::Log((String*)0, 1);
		Debug::Log((String)"΢���ĵ���Զ����" + false + 123);
	}

	static void TestWindowBase()
	{
		WindowBase window(L"���Գɹ�");
		TestWindow(window);
	}

	static void TestFileOpenDialog()
	{
		Debug::Log(WindowUtility::GetOpenFilePath());
	}

	static void TestWindow(WindowBase& window)
	{
		window.Show();

		//������Ϣ����
		MSG msg = {};
		while (GetMessage(&msg, NULL, 0, 0))
		{
			//Ԥ����󽻸����ڹ�����Ӧ
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
};

