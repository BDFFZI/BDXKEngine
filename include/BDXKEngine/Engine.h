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
#include "BDXKEngine/Base/Object/ObjectPtr.h"
#include "BDXKEngine/Base/Color.h"
#include "BDXKEngine/Base/Rect.h"
#include "BDXKEngine/Base/Vector/Vector2.h"
#include "BDXKEngine/Base/Vector/Vector3.h"
#include "BDXKEngine/Base/Matrix/Matrix3x2.h"
#include "BDXKEngine/Base/Matrix/Matrix4x4.h"
#include "BDXKEngine/Base/Extension/String.h"
#include "BDXKEngine/Base/Extension/Debug.h"
//平台层：对操作系统以及各种非标准库的封装 >> 引入第三方，接入公共协议
#include "BDXKEngine/Platform/Window/Window.h"
#include "BDXKEngine/Platform/GL/GL.h"
#include "BDXKEngine/Platform/GL/GL2D.h"
#include "BDXKEngine/Platform/GL/Resources/Mesh.h"
#include "BDXKEngine/Platform/GL/Resources/Shader.h"
#include "BDXKEngine/Platform/GL/Resources/Texture2D.h"
#include "BDXKEngine/Platform/GL/Resources/TextureCube.h"
#include "BDXKEngine/Platform/IMGUI/IMGUIManager.h"
#include "BDXKEngine/Platform/IO/MeshImporter.h"
//功能层：引擎运行中提供的各种运行时功能 >> 封装底层，建立实用函数
#include "BDXKEngine/Platform/IO/Resources.h"//对各种外部文件包括引擎持久化资源的表示和读写 >> 引擎启动和运行的数据源头，未来编辑器的编辑目标
#include "BDXKEngine/Function/Graphics.h"
#include "BDXKEngine/Function/Time.h"
#include "BDXKEngine/Function/Input.h"
#include "BDXKEngine/Function/Screen.h"
#include "BDXKEngine/Function/Cursor.h"
#include "BDXKEngine/Function/Event.h"
#include "BDXKEngine/Function/GUI.h"
#include "BDXKEngine/Function/Random.h"
//框架层：在此组织和使用下层的各种功能 >> 营造引擎环境，用户接口
#include "BDXKEngine/Framework/GameObject/GameObject.h"
#include "BDXKEngine/Framework/Component/Component.h"
#include "BDXKEngine/Framework/Component/Renderer/RenderManager.h"
#include "BDXKEngine/Framework/Component/Behavior/BehaviorManager.h"
#include "BDXKEngine/Framework/Component/Transform.h"
#include "BDXKEngine/Framework/Component/Renderer/MeshRenderer.h"
#include "BDXKEngine/Framework/Component/Renderer/Camera.h"
#include "BDXKEngine/Framework/Component/Renderer/RenderEvent.h"
#include "BDXKEngine/Framework/Component/Behavior/Animator.h"
#include "BDXKEngine/Framework/Scene/GraphicsSettings.h"
#include "BDXKEngine/Framework/Scene/Scene.h"


namespace BDXKEngine
{
    class Engine : Resources, Time, Input, Screen, Cursor, Event, Graphics, GUI, TransformManager
    {
    public:
        static void Run(std::function<void(const ObjectPtr<Scene>& scene)> onStart)
        {
            std::setlocale(LC_ALL, "zh-CN.UTF-8");

            //启动窗口
            Window window{L"BDXKEngine"};

            //这三层是静态的，所以需要顺序初始化，以便初始化无异常
            {
                //基础层初始化
                Object::SetCreator<Shader>();
                Object::SetCreator<Buffer>();
                Object::SetCreator<Texture2D>();
                Object::SetCreator<TextureCube>();
                Object::SetCreator<Material>();
                Object::SetCreator<Mesh>();

                Object::SetCreator<Transform>();
                Object::SetCreator<Component>();
                Object::SetCreator<GameObject>();
                Object::SetCreator<Scene>();
                Object::SetCreator<QualitySettings>();
                Object::SetCreator<GraphicsSettings>();

                Object::SetCreator<Animator>();
                Object::SetCreator<AnimatorClip>();
                Object::SetCreator<Camera>();
                Object::SetCreator<Light>();
                Object::SetCreator<MeshRenderer>();
                Object::SetCreator<ScriptableObject>();

                //平台层初始化
                GL::Initialize(&window);
                GL2D::Initialize();
                IMGUIManager::Initialize(&window);

                //资源层初始化
                Resources::Initialize(&window);
            }

            //下面两层是动态的，倒序初始化（因为会影响事件顺序），以便使用环境无异常
            BehaviorManager::Initialize(&window);
            RenderManager::Initialize(&window);
            GUI::Initialize(&window);
            Event::Initialize(&window);
            Graphics::Initialize(&window);
            Screen::Initialize(&window);
            Cursor::Initialize(&window);
            Input::Initialize(&window);
            Time::Initialize(&window);

            {
                const auto scene = Scene::Create();

                //完成初始化后，正式循环前，触发事件回调
                onStart(scene);
                //正式循环
                window.Show();

                Object::DestroyImmediate(scene.ToObjectBase());
            }

            Debug::LogError("系统回收检查");
            Object::DebugObjectCount();
            ObjectPtrBase::DebugRefCountMap();
            BehaviorManager::DebugHandlersCount();
        }
    };
}
