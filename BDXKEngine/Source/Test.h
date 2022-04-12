#pragma once
#include "BDXKEngine.h"


class Test {
public:
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

	static void TestGameObject()
	{
		BDXKEngine::Awake();

		//�ڴ˴�����ʼ����
		GameObject* gameObject = new GameObject(L"BDXKEngine");
		//���1
		gameObject->AddComponent<TestComponent>();
		//���2
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
			Debug::Log(L"�ҳ�����");
			Debug::LogError((String)L"�ҵ�������" + GetGameObject()->name);
		}

		void OnStart()override {
			GameObject* gameObject = GetGameObject();
			Debug::LogWarning(String("���Ѿ�׼�����ں�����") + x + "����ͼ");
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

