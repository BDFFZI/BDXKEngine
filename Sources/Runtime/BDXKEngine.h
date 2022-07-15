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

//基础数据结构
#include "Color.h"
#include "Rect.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Matrix3x2.h"
#include "Matrix4x4.h"
//工具
#include "Math.h"
#include "String.h"
#include "Debug.h"
#include "WindowBase.h"
#include "Window.h"
//DirectX接口封装
#include "GL.h"
#include "GL2D.h"
//DirectX接口封装数据结构
#include "Mesh.h"
#include "Shader.h"
#include "Texture2D.h"
//系统
#include "Graphics.h"
#include "Time.h"
#include "Input.h"
#include "Screen.h"
#include "Cursor.h"
#include "Event.h"
#include "GUI.h"
//组件
#include "Object.h"
#include "GameObject.h"
#include "Component.h"
#include "Transform.h"
#include "MeshRenderer.h"
#include "Camera.h"
#include "Animator.h"

namespace BDXKEngine {
	class Engine :Time, Input, Screen, Cursor, Graphics, Event, GUI, GameObjectEditor, TransformEditor {
	public:
		static void Run(::std::function<void()> onStart)
		{
			std::setlocale(LC_ALL, "zh-CN");

			//启动窗口
			Window window = {
				L"BDXKEngine",
				[&](Window* window, UINT messageSign, WPARAM wparameter, LPARAM lparameter) {
					switch (messageSign)
					{
					case WM_CREATE:
					{
						//系统初始化
						Time::Initialize(window);
						Screen::Initialize(window);
						Input* input = Input::Initialize(window);
						Cursor* cursor = Cursor::Initialize(input,window);
						Event* event = Event::Initialize(input, window);
						GL* gl = nullptr;
						GL2D* gl2d = nullptr;
						Graphics* graphics = Graphics::Initialize(window, &gl, &gl2d);
						GUI::Initialize(gl2d, event, window);

						//完成初始化后，正式循环前
						onStart();
						break;
					}
					case WM_PAINT:
					{
						//帧更新
						GameObjectEditor::OnUpdate();
						break;
					}
					//case WM_CLOSE:
					//{
					//	if (MessageBox(window->GetHwnd(), L"确定关闭？", L"关闭窗口", MB_OKCANCEL) == IDOK)
					//		DestroyWindow(window->GetHwnd());
					//	return LRESULT{0};
					//}
					//case WM_DESTROY:
					//{
					//	PostQuitMessage(0);
					//	break;
					//}
					}

					return DefWindowProcW(window->GetHwnd(), messageSign, wparameter, lparameter);
				}
			};
			window.Show();

			//正式循环
			HWND hwnd = window.GetHwnd();
			MSG msg = {};
			while (GetMessage(&msg, hwnd, 0, 0) > 0)
			{
				//预处理后交给窗口过程响应
				TranslateMessage(&msg);
				DispatchMessage(&msg);

				//如果没有要处理的消息，我们就用这段空闲时间更新游戏
				if (PeekMessage(&msg, hwnd, NULL, NULL, NULL) == FALSE)
					PostMessage(hwnd, WM_PAINT, NULL, NULL);
			}
		}
	};
}
