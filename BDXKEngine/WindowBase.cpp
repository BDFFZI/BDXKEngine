#include "WindowBase.h"
#include <iostream>
using namespace std;

WindowBase::WindowBase(PCWSTR windowName)
{
	WindowName = windowName;
	hwnd = NULL;
}

BOOL WindowBase::Show()
{
	//向操作系统注册窗口类
	WNDCLASS windowClass = {};
	windowClass.lpszClassName = WindowName;//类名，标识
	windowClass.lpfnWndProc = WindowProcess;//窗口消息处理函数
	RegisterClass(&windowClass);

	//创建窗口
	hwnd = CreateWindowEx(
		0,//窗口行为
		WindowName, //窗口类名称
		WindowName,//窗口标题
		WS_OVERLAPPEDWINDOW,//窗口样式
		CW_USEDEFAULT, CW_USEDEFAULT,//窗口位置xy
		CW_USEDEFAULT, CW_USEDEFAULT,//窗口大小xy
		NULL,//父窗口
		NULL,//窗口菜单
		NULL,//实例句柄,默认
		this//附加的数据
	);
	if (hwnd != NULL)
	{
		ShowWindow(hwnd, SW_SHOWDEFAULT);
		return TRUE;
	}

	return FALSE;
}

PCWSTR WindowBase::GetWindowName()
{
	return WindowName;
}

HWND WindowBase::GetHWND()
{
	return hwnd;
}

LRESULT WindowBase::HandleMessage(UINT messageSign, WPARAM wparameter, LPARAM lparameter)
{
	switch (messageSign)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hwnd, &ps);

		FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_HOTLIGHT));

		EndPaint(hwnd, &ps);
	}
	return 0;
	}
}

LRESULT WindowBase::WindowProcess(HWND hwnd, UINT messageSign, WPARAM wparameter, LPARAM lparameter)
{
	WindowBase* window = NULL;

	//获取窗口实例
	if (messageSign == WM_NCCREATE)
	{
		//第一次需转存实例
		CREATESTRUCT* createStruct = (CREATESTRUCT*)lparameter;
		window = (WindowBase*)createStruct->lpCreateParams;
		SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)window);
	}
	else
	{
		window = (WindowBase*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
	}

	if (window != NULL)
	{
		//使用窗口实例提供的消息处理程序
		return	window->HandleMessage(messageSign, wparameter, lparameter);
	}
	else
	{
		return	DefWindowProc(hwnd, messageSign, wparameter, lparameter);
	}
}