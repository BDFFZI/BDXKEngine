#include "BDXKEngine.h"

MainWindow* BDXKEngine::window = NULL;

void BDXKEngine::Awake()
{
	std::setlocale(LC_ALL, "zh-CN");

	window = new MainWindow();
	window->Show();
	//Graphics::SetRenderTarget(window->GetHwnd()); ���ڴ����г�ʼ��

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
	//������Ϣ����
	HWND hwnd = window->GetHwnd();
	MSG msg = {};
	while (true)
	{
		//��֤��Ϣ���в�Ϊ�գ��Ӷ�һֱ���´���
		if (PeekMessage(&msg, hwnd, NULL, NULL, NULL) == FALSE)
			PostMessage(hwnd, WM_PAINT, NULL, NULL);

		//��ȡ��Ϣ
		GetMessage(&msg, hwnd, 0, 0);
		//Ԥ����󽻸����ڹ�����Ӧ
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}
