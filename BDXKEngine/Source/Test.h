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
			//键盘
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

			//鼠标
			size += Input::GetMouseScrollDelta().y;

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
			Debug::Log(L"我出生了");
			Debug::LogError((String)L"我的名字是" + GetGameObject()->name);
		}

		void Start()override {
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
		HRESULT result = CoInitialize(NULL);//初始化COM环境
		if (FAILED(result)) {
			throw std::domain_error("COM初始化失败。");
		}

		IFileOpenDialog* dialog;
		result = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_INPROC_SERVER, IID_IFileOpenDialog, reinterpret_cast<void**>(&dialog));//创建COM对象并获取接口
		if (FAILED(result)) {
			CoUninitialize();
			throw std::domain_error("对话框创建失败。");
		}

		//查询该对象的其他接口
		//IFileDialogCustomize* customize;
		//result = dialog->QueryInterface(IID_IFileDialogCustomize, reinterpret_cast<void**>(&customize));


		dialog->Show(NULL);
		IShellItem* shellItem;
		result = dialog->GetResult(&shellItem);
		if (FAILED(result)) {
			dialog->Release();
			CoUninitialize();
			throw std::domain_error("获取对话结果失败。");
		}

		PWSTR str;
		result = shellItem->GetDisplayName(SIGDN_FILESYSPATH, &str);//调用COM函数，在堆上分配内存
		if (FAILED(result)) {
			shellItem->Release();
			dialog->Release();
			CoUninitialize();
			throw std::domain_error("获取对话结果失败。");
		}

		String path(str);
		CoTaskMemFree(str);//释放堆内存

		shellItem->Release();
		dialog->Release();//释放COM对象
		CoUninitialize();//退出COM环境

		Debug::Log(path);
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

