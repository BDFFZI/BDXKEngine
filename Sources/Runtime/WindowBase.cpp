#include "WindowBase.h"

namespace BDXKEngine {
	WindowBase::WindowBase(PCWSTR name)
	{
		this->name = name;
		this->hwnd = NULL;
	}

	void WindowBase::Show()
	{
		if (hwnd != NULL)
			throw "窗口正在显示中";

		//向操作系统注册窗口类
		WNDCLASS windowClass = {};
		windowClass.lpszClassName = name;//类名，标识
		windowClass.lpfnWndProc = WindowProcess;//窗口消息处理函数
		RegisterClass(&windowClass);

		//创建窗口
		HWND hwnd = CreateWindowEx(
			0,//窗口行为
			name, //窗口类名称
			name,//窗口标题
			WS_OVERLAPPEDWINDOW,//窗口样式
			CW_USEDEFAULT, CW_USEDEFAULT,//窗口位置xy
			CW_USEDEFAULT, CW_USEDEFAULT,//窗口大小xy
			NULL,//父窗口
			NULL,//窗口菜单
			NULL,//实例句柄,默认
			this//附加的数据
		);

		//显示窗口
		ShowWindow(hwnd, SW_SHOWDEFAULT);
	}

	PCWSTR WindowBase::GetName()
	{
		return name;
	}

	HWND WindowBase::GetHwnd()
	{
		return hwnd;
	}

	LRESULT CALLBACK WindowBase::HandleMessage(UINT messageSign, WPARAM wparameter, LPARAM lparameter)
	{
		return DefWindowProc(hwnd, messageSign, wparameter, lparameter);
	}

	LRESULT CALLBACK WindowBase::WindowProcess(HWND hwnd, UINT messageSign, WPARAM wparameter, LPARAM lparameter)
	{
		WindowBase* window = NULL;

		//获取窗口实例
		if (messageSign == WM_NCCREATE)
		{
			//第一次需转存实例
			CREATESTRUCT* createStruct = (CREATESTRUCT*)lparameter;
			window = (WindowBase*)createStruct->lpCreateParams;
			SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)window);
			window->hwnd = hwnd;
		}
		else
		{
			window = (WindowBase*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
		}


		if (window != NULL)
		{
			//使用窗口实例提供的消息处理程序
			return window->HandleMessage(messageSign, wparameter, lparameter);
		}
		else
		{
			return DefWindowProc(hwnd, messageSign, wparameter, lparameter);
		}
	}
}