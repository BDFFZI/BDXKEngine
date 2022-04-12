#include "BDXKEngine.h"

MainWindow* BDXKEngine::window = NULL;

void BDXKEngine::Awake()
{
	std::setlocale(LC_ALL, "zh-CN");

	window = new MainWindow();
	window->Show();
	//Graphics::SetRenderTarget(window->GetHwnd()); 已在窗口中初始化

	Time::Awake();
}

void BDXKEngine::Update()
{
	Time::BeginFrame();
	Graphics::BeginDraw();

	Graphics::ClearCanvas();

	for (GameObject* gameObject : GameObject::gameObjects)
	{
		gameObject->OnUpdate();
	}

	Graphics::EndDraw();
	Time::EndFrame();
}

void BDXKEngine::Run()
{
	//检索消息队列
	HWND hwnd = window->GetHwnd();
	MSG msg = {};
	while (true)
	{
		//保证消息队列不为空，从而一直更新窗口
		if (PeekMessage(&msg, hwnd, NULL, NULL, NULL) == FALSE)
			PostMessage(hwnd, WM_PAINT, NULL, NULL);

		//获取消息
		GetMessage(&msg, hwnd, 0, 0);
		//预处理后交给窗口过程响应
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}
