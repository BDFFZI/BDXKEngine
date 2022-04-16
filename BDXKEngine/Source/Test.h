#pragma once
#include "BDXKEngine.h"


class Test {
public:


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
			if (rotate)Graphics::SetTransform(Matrix3x2::Rotate(Time::GetRealtimeSinceStartup() * 10));
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

