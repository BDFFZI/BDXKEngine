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

	static void TestMainWindow()
	{
		MainWindow window;
		TestWindow(window);
	}

	static void TestFileOpenDialog()
	{
		Debug::Log(DialogUtility::FileOpenDialog());
	}
private:
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

