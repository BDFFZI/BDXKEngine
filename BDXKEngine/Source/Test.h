#pragma once
#include "BDXKEngine.h"


class Test {
public:
	class TestMatrixAndInput :public Component {
	private:
		Vector2 center = Vector2(Vector2::one) * 100;
		float size = 1;
		float sleep = 100;


		void Update()override
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
			size += Input::GetMouseScrollDelta().y;

			if (Input::GetMouseButtonDown(0))
			{
				Debug::Log("����", 4);
				Cursor::SetLockState(true);
			}
			if (Input::GetMouseButtonUp(0))
			{
				Debug::Log("����", 2);
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

			Graphics::SetMatrix(matrix);
			Graphics::SetBrushColor(Color::blue);

			Graphics::DrawRectangleCenter(Vector2(0, 0), Vector2(100, 50), true);
		}
	};

	class TestObjectAndGraphics :public Component
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

		void Start()override {
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
			if (rotate)Graphics::SetMatrix(Matrix3x2::Rotate(Time::GetRealtimeSinceStartup() * 10));
			Graphics::SetBrushColor(Color::blue);
			Graphics::DrawRectangle(Vector2(x, 100), Vector2(x + 50, 300), false);
			Graphics::SetMatrix();
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
		HRESULT result = CoInitialize(NULL);//��ʼ��COM����
		if (FAILED(result)) {
			throw std::domain_error("COM��ʼ��ʧ�ܡ�");
		}

		IFileOpenDialog* dialog;
		result = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_INPROC_SERVER, IID_IFileOpenDialog, reinterpret_cast<void**>(&dialog));//����COM���󲢻�ȡ�ӿ�
		if (FAILED(result)) {
			CoUninitialize();
			throw std::domain_error("�Ի��򴴽�ʧ�ܡ�");
		}

		//��ѯ�ö���������ӿ�
		//IFileDialogCustomize* customize;
		//result = dialog->QueryInterface(IID_IFileDialogCustomize, reinterpret_cast<void**>(&customize));


		dialog->Show(NULL);
		IShellItem* shellItem;
		result = dialog->GetResult(&shellItem);
		if (FAILED(result)) {
			dialog->Release();
			CoUninitialize();
			throw std::domain_error("��ȡ�Ի����ʧ�ܡ�");
		}

		PWSTR str;
		result = shellItem->GetDisplayName(SIGDN_FILESYSPATH, &str);//����COM�������ڶ��Ϸ����ڴ�
		if (FAILED(result)) {
			shellItem->Release();
			dialog->Release();
			CoUninitialize();
			throw std::domain_error("��ȡ�Ի����ʧ�ܡ�");
		}

		String path(str);
		CoTaskMemFree(str);//�ͷŶ��ڴ�

		shellItem->Release();
		dialog->Release();//�ͷ�COM����
		CoUninitialize();//�˳�COM����

		Debug::Log(path);
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

namespace TestRelation {
	class Controler :public Component
	{
	public:
		Transform* target;
	private:
		Transform* transform = NULL;
		float scale = 1;

		void Start()override
		{
			transform = GetGameObject()->GetTransform();
		}

		void Update()override
		{
			scale = Math::Clamp(scale + Input::GetMouseScrollDelta().y / 10, 0, 2);

			transform->SetLocalScale((Vector2)Vector2::one * (scale + 0.1f));
			transform->SetLocalEulerAngles({ 0,0,Time::GetRealtimeSinceStartup() * 100 });
			transform->SetLocalPosition(Input::GetMousePosition());

			target->SetLocalEulerAngles({ 0,0,Time::GetRealtimeSinceStartup() * -100 });
			target->SetLocalScale((Vector2)Vector2::one * (2 - scale + 0.1f));
		}
	};

	void Start()
	{
		BDXKEngine::Run([]() {
			GameObject* circle = new GameObject(L"circle");
			Renderer* circleRenderer = circle->AddComponent<Renderer>();
			circleRenderer->render = []() {Graphics::DrawCircle(Vector2::zero, 10, true); };
			circleRenderer->color = Color::red;
			Controler* testComponent = circle->AddComponent<Controler>();

			GameObject* square = new GameObject(L"square");
			Renderer* squareRenderer = square->AddComponent<Renderer>();
			squareRenderer->render = []() {Graphics::DrawRectangleCenter(Vector2::zero, (Vector2)Vector2::one * 100, false); };
			squareRenderer->color = Color::green;
			Transform* squareTransform = square->GetTransform();
			squareTransform->SetLocalPosition({ 50,50,0 });
			squareTransform->SetParent(circle->GetTransform());

			GameObject* rectangle = new GameObject(L"rectangle");
			Renderer* rectangleRenderer = rectangle->AddComponent<Renderer>();
			rectangleRenderer->render = []() {Graphics::DrawRectangle(Vector2::zero, Vector2(100, 20), false); };
			rectangleRenderer->color = Color::blue;
			Transform* rectangleTransform = rectangle->GetTransform();
			rectangleTransform->SetLocalPosition({ 50,50,0 });
			rectangleTransform->SetParent(squareTransform);

			testComponent->target = rectangleTransform;
			});
	}
}

