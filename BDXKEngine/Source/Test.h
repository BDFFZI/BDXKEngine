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

