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

//基础层：公共数据类型，统一各模块交流的语言 >> 扩展标准库
#include "ObjectPtr.h"
#include "Color.h"
#include "Rect.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Matrix3x2.h"
#include "Matrix4x4.h"
//平台层：对操作系统以及各种非标准库的封装 >> 扩展标准库
#include "String.h"
#include "Debug.h"
#include "WindowBase.h"
#include "Window.h"
#include "GL.h"
#include "GL2D.h"
#include "Mesh.h"
#include "Shader.h"
#include "Texture2D.h"
//资源层：对各种外部文件包括引擎持久化资源的表示和读写 >> 引擎启动和运行的数据源头，未来编辑器的编辑目标
#include "Resources.h"
#include "MeshImporter.h"
//功能层：引擎运行中提供的各种运行时功能 >> 引擎可以正常运行了
#include "Graphics.h"
#include "Time.h"
#include "Input.h"
#include "Screen.h"
#include "Cursor.h"
#include "Event.h"
#include "GUI.h"
#include "Random.h"
//框架层：在此组织和使用下层的各种功能 >> 用户控制引擎的接口
#include "GameObjectManager.h"
#include "GameObject.h"
#include "Component.h"
#include "Transform.h"
#include "MeshRenderer.h"
#include "Camera.h"
#include "Animator.h"

namespace BDXKEngine {
	class Engine :Time, Input, Screen, Cursor, Event, Graphics, GUI, TransformEditor, GameObjectManager {
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
						GameObjectManager* gameObjectManager = GameObjectManager::Initialize(window);
						//功能初始化
						Time::Initialize(window);
						Screen::Initialize(window);
						Input* input = Input::Initialize(window);
						Cursor* cursor = Cursor::Initialize(input,window);
						Event* event = Event::Initialize(input, window);
						GL* gl = GL::Initialize(window->GetHwnd());
						GL2D* gl2d = nullptr;
						Graphics* graphics = Graphics::Initialize(window, gl, new Material{ {
								new Shader(
									GetResourcesPathW(Shaders, Blit\\VertexShader.hlsl),
									GetResourcesPathW(Shaders, Blit\\PixelShader.hlsl),
									PassType::ForwardBase
								)
							} },
							&gl2d);
						GUI* gui = GUI::Initialize(gl2d, event, window);

						//创建配置信息，这将影响框架层中部分模块使用功能层的方式
						CreateSettings();

						//完成初始化后，正式循环前，触发事件回调
						onStart();

						break;
					}
					case WM_DESTROY:
					{
						ReleaseSettings();
						break;
					}
					//case WM_CLOSE:
					//{
					//	if (MessageBox(window->GetHwnd(), L"确定关闭？", L"关闭窗口", MB_OKCANCEL) == IDOK)
					//		DestroyWindow(window->GetHwnd());
					//	return LRESULT{0};
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
	private:
		static void CreateSettings() {
			GraphicsSettings::shadowMapMaterial = new Material{ {
				new Shader(
					GetResourcesPathW(Shaders, ShadowMap\\VertexShader.hlsl),
					GetResourcesPathW(Shaders, ShadowMap\\PixelShader.hlsl),
					PassType::ForwardBase
				)
			} };
		}
		static void ReleaseSettings()
		{
			Object::Destroy(GraphicsSettings::shadowMapMaterial.GetPtr());
		}
	};
}
