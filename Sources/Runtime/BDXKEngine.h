// ReSharper disable CppUnusedIncludeDirective
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

//基础层：公共数据类型，统一各模块交流的语言 >> 扩展标准库，建立公共协议
#include "Base/Object/ObjectPtr.h"
#include "Base/Object/ObjectEvent.h"
#include "Base/Color.h"
#include "Base/Rect.h"
#include "Base/Vector/Vector2.h"
#include "Base/Vector/Vector3.h"
#include "Base/Matrix/Matrix3x2.h"
#include "Base/Matrix/Matrix4x4.h"
#include "Base/Extension/String.h"
#include "Base/Extension/Debug.h"
//平台层：对操作系统以及各种非标准库的封装 >> 引入第三方，接入公共协议
#include "Platform/Window/WindowBase.h"
#include "Platform/Window/Window.h"
#include "Platform/GL/GL.h"
#include "Platform/GL/GL2D.h"
#include "Platform/GL/Resources/Mesh.h"
#include "Platform/GL/Resources/Shader.h"
#include "Platform/GL/Resources/Texture2D.h"
#include "Platform/GL/Resources/TextureCube.h"
#include "Platform/IO/MeshImporter.h"
//功能层：引擎运行中提供的各种运行时功能 >> 封装底层，建立实用函数
#include "Function/Resources.h"//对各种外部文件包括引擎持久化资源的表示和读写 >> 引擎启动和运行的数据源头，未来编辑器的编辑目标
#include "Function/Graphics.h"
#include "Function/Time.h"
#include "Function/Input.h"
#include "Function/Screen.h"
#include "Function/Cursor.h"
#include "Function/Event.h"
#include "Function/GUI.h"
#include "Function/Random.h"
//框架层：在此组织和使用下层的各种功能 >> 营造引擎环境，用户接口
#include "Framework/WorldManager.h"
#include "Framework/GameObject.h"
#include "Framework/Component.h"
#include "Framework/Components/Renderer/RendererManager.h"
#include "Framework/Components/Behavior/BehaviorManager.h"
#include "Framework/Components/Transform.h"
#include "Framework/Components/Renderer/MeshRenderer.h"
#include "Framework/Components/Renderer/Camera.h"
#include "Framework/Components/Renderer/CameraEvent.h"
#include "Framework/Components/Behavior/Animator.h"


namespace BDXKEngine
{
    class Engine : Resources, Time, Input, Screen, Cursor, Event, Graphics, GUI, TransformEditor, WorldManager
    {
    public:
        static void Run(std::function<void()> onStart)
        {
            std::setlocale(LC_ALL, "zh-CN");

            //启动窗口
            Window window = {L"BDXKEngine"};

            //平台层初始化
            GL::Initialize(&window);
            GL2D::Initialize();
            //资源层初始化
            Resources::Initialize(&window);
            //上面三层是静态的，顺序初始化，以便初始化无异常

            //创建配置信息，这将影响框架层中部分模块的运作方式
            CreateSettings();

            //下面两层是动态的，倒序初始化（因为会影响事件顺序），以便使用环境无异常
            BehaviorManager::Initialize(&window);
            RendererManager::Initialize(&window);
            WorldManager::Initialize(&window);
            GUI::Initialize(&window);
            Event::Initialize(&window);
            Graphics::Initialize(&window);
            Screen::Initialize(&window);
            Cursor::Initialize(&window);
            Input::Initialize(&window);
            Time::Initialize(&window);

            //完成初始化后，正式循环前，触发事件回调
            onStart();

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

            ReleaseSettings();

            Debug::LogError(L"系统回收检查");
            Object::DebugObjectCount();
            ObjectPtrBase::DebugRefCountMap();
            BehaviorManager::DebugHandlersCount();
        }

    private:
        static void CreateSettings()
        {
        }

        static void ReleaseSettings()
        {
            GraphicsSettings::skybox = nullptr;
        }
    };
}
