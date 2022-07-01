#pragma once
#include "BDXKEngine.h"

class Test {
public:
	class TestMatrixAndInput :public Component, UpdateEvent, RenderObjectEvent
	{
	private:
		Vector2 center = Vector2(Vector2::one) * 100;
		float size = 1;
		float sleep = 100;


		void OnUpdate()override
		{
			//����
			float move = Time::GetDeltaTime() * 100;
			if (Input::GetKey(KeyCode::Shift))
			{
				Debug::Log(L"����");
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

			//���
			size += Input::GetMouseScrollDelta();

			if (Input::GetMouseButtonDown(0))
			{
				Cursor::SetLockState(true);
			}
			if (Input::GetMouseButtonUp(0))
			{
				Cursor::SetLockState(false);
			}

			if (Input::GetMouseButton(1))
				center = Input::GetMousePosition();
			if (Input::GetMouseButtonDown(1)) Cursor::SetVisible(false);
			if (Input::GetMouseButtonUp(1))Cursor::SetVisible(true);
		}

		void OnRenderObject()override
		{
			Matrix3x2 matrix = Matrix3x2::identity;
			matrix *= Matrix3x2::Scale((Vector2)Vector2::one * size);
			matrix *= Matrix3x2::Rotate(Time::GetRealtimeSinceStartup() * sleep);
			matrix *= Matrix3x2::Translate(center);

			GL2D::SetMatrix(matrix);
			GL2D::SetBrushColor(Color::blue);

			GL2D::DrawRectangleCenter(Vector2(0, 0), Vector2(100, 50), true);
		}
	};

	class TestObjectAndGraphics :public Component,
		AwakeEvent, StartEvent, UpdateEvent, RenderObjectEvent, DrawGizmosEvent
	{
	public:
		float x = 100;
		bool rotate = false;

	private:
		float y = 0;

		void OnAwake()override {
			Debug::Log(L"�ҳ�����");
			Debug::LogError((String)L"�ҵ�������" + GetGameObject()->GetName());
		}

		void OnStart()override {
			GameObject* gameObject = GetGameObject();
			Debug::LogWarning(String("���Ѿ�׼�����ں�����") + x + "����ͼ");
		}

		void OnUpdate()override {
			y = std::sin(Time::GetRealtimeSinceStartup()) * 100;
		}

		void OnRenderObject() override {
			GL2D::SetBrushColor({ 0, 1, 0, 0.5f });
			GL2D::DrawCircle({ x,200 + y }, 100, true);
			GL2D::SetBrushColor({ 1, 0, 0, 0.5f });
			GL2D::DrawCircle({ x + 50,200 + (1 - y) }, 100, true);
		}

		void OnDrawGizmos() override
		{
			if (rotate)GL2D::SetMatrix(Matrix3x2::Rotate(Time::GetRealtimeSinceStartup() * 10));
			GL2D::SetBrushColor(Color::blue);
			GL2D::DrawRectangle(Vector2(x, 100), Vector2(x + 50, 300), false);
			GL2D::SetMatrix();
		}
	};

	static void TestDebug()
	{
		Debug::Log((String)true);
		Debug::LogWarning((String)23.23f);
		Debug::LogError(L"�ı�");
		Debug::Log((String)0, 1);
		Debug::Log((String)"΢���ĵ���Զ����" + false + 123);
	}

	class TestRender :public GL
	{
		void Test()
		{
			std::setlocale(LC_ALL, "zh-CN");

			WindowBase window(L"Direct3D Test");
			window.Show();

			GL::Initialize(window.GetHwnd());

			Mesh mesh{};
			Shader shader{};

			//������Ϣ����
			MSG msg = {};
			while (GetMessage(&msg, NULL, 0, 0))
			{
				//Ԥ����󽻸����ڹ�����Ӧ
				TranslateMessage(&msg);
				DispatchMessage(&msg);
				Graphics::RenderMesh(&mesh, &shader);
			}
		}
	};

	static void TestWindowBase()
	{
		WindowBase window(L"���Գɹ�");
		TestWindow(window);
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

