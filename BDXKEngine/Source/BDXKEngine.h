#pragma once
//C++匈牙利表示法，允许组合使用
//W 宽，如wchar_t,以处理Unicode中utf-16,注意现在都用这个
//L 长，x16时代的产物，现在不带也是长
//P 指针
//H 句柄
//LP 长指针
//C 常量

//C++特殊函数后缀，代表函数的不同版本
// 
//函数中使用的字符类型
//a ASCII编码
//W 宽字符编码，咱中文就用这个
// 
//一般表明颜色存储方式
//U	无符号整数 如：0-255
//F 浮点数 如：0.0-1.0

//C++数据类型
//PCWSTR = 常量宽字符指针,标准字符串
//LRESULT = long int，表示函数的返回代码
//HWND = 窗口句柄
//BOOL = int
//WPARAM = uint
//LPARAM = int

//基本
#include<algorithm>
#include "Com.h"
#include "String.h"
#include "Object.h"
#include "List.h"
#include "Color.h"
#include "Rect.h"
#include "Math.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Matrix3x2.h"
#include "Matrix4x4.h"
//工具
#include "Debug.h"
#include "Assert.h"
#include "WindowBase.h"
#include "Window.h"
//系统
#include "Graphics.h"
#include "Time.h"
#include "Input.h"
#include "Screen.h"
#include "Cursor.h"
//组件
#include "GameObject.h"
#include "Component.h"
#include "ComponentEvent.h"
#include "Transform.h"
#include "Renderer.h"

class BDXKEngine {
public:
	static void Run(std::function<void()> onStart)
	{
		std::setlocale(LC_ALL, "zh-CN");

		//初始化
		Window window = Window(L"BDXKEngine",
			[](Window* window, UINT messageSign, WPARAM wparameter, LPARAM lparameter) {
				switch (messageSign)
				{
				case WM_CREATE:
				{
					Graphics::SetRenderTarget(window->GetHwnd());
					Screen::Initialize(window);
					Time::Initialize();
					return true;
				}
				case WM_PAINT:
				{
					Time::BeginFrame();
					Graphics::BeginDraw();
					Graphics::ClearCanvas();

					////统计待更新的物体，以层次结构为优先级
					//List<Transform*> list;
					//list.push_back(&Transform::root);
					//for (int i = 0; i < list.size(); i++)
					//{
					//	for (Transform* child : list[i]->children)
					//	{
					//		list.push_back(child);
					//	}
					//}
					////更新物体，（第一个为系统根物体，跳过）
					//std::for_each(
					//	(list.begin() + 1), list.end(),
					//	[](Transform* item) {
					//		item->GetGameObject()->Update();
					//	}
					//);
					GameObject::Update();

					Graphics::EndDraw();
					Time::EndFrame();
					Input::Update();
					return true;
				}
				case WM_SIZE:
				{
					Graphics::ResetCanvas();
					window->RePaint();
					return true;
				}
				case WM_SETCURSOR:
				{
					if (LOWORD(lparameter) == HTCLIENT)
					{
						Cursor::Update();
						return true;
					}
				}
#pragma region 鼠标
				case WM_MOUSEMOVE:
				{
					Input::mousePosition.x = (float)(lparameter << 48 >> 48);
					Input::mousePosition.y = (float)(lparameter >> 16);
					return true;
				}
				case WM_MOUSEWHEEL:
				{
					Input::mouseScrollDelta.y += GET_WHEEL_DELTA_WPARAM(wparameter) / 1000.0f;
					return true;
				}
				case WM_LBUTTONDOWN:
				{
					Input::mouseButtonState[0] = true;
					SetCapture(window->GetHwnd());
					return true;
				}
				case WM_LBUTTONUP:
				{
					Input::mouseButtonState[0] = false;
					if (Input::GetMouseButton(1) == false &&
						Input::GetMouseButton(2) == false)
						ReleaseCapture();
					return true;
				}
				case WM_RBUTTONDOWN:
				{
					Input::mouseButtonState[1] = true;
					SetCapture(window->GetHwnd());
					return true;
				}
				case WM_RBUTTONUP:
				{
					Input::mouseButtonState[1] = false;
					if (Input::GetMouseButton(0) == false &&
						Input::GetMouseButton(2) == false)
						ReleaseCapture();
					return true;
				}
				case WM_MBUTTONDOWN:
				{
					Input::mouseButtonState[2] = true;
					SetCapture(window->GetHwnd());
					return true;
				}
				case WM_MBUTTONUP:
				{
					Input::mouseButtonState[2] = false;
					if (Input::GetMouseButton(0) == false &&
						Input::GetMouseButton(1) == false)
						ReleaseCapture();
					return true;
				}
#pragma endregion
#pragma region 键盘
				case WM_KEYDOWN:
				{
					Input::keyboardState[wparameter] = true;
					return true;
				}
				case WM_KEYUP:
				{
					Input::keyboardState[wparameter] = false;
					return true;
				}
#pragma endregion
				case WM_CLOSE:
				{
					if (MessageBox(window->GetHwnd(), L"确定关闭？", L"关闭窗口", MB_OKCANCEL) == IDOK)
						DestroyWindow(window->GetHwnd());
					return true;
				}
				case WM_DESTROY:
				{
					PostQuitMessage(0);
					return true;
				}
				}
				return false;
			});
		window.Show();

		//完成初始化后，正式循环前
		onStart();

		//正式循环
		HWND hwnd = window.GetHwnd();
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
};