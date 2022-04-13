#pragma once
#include "BDXKEngine.h"


class Test {
public:
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
		Debug::Log(WindowUtility::GetOpenFilePath());
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

