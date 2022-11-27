#include "BDXKEngine.h"
#include "Framework/Scene.h"
#include "Framework/Behavior/BehaviorEvent.h"
#include "Framework/Renderer/RenderEvent.h"
#include "Function/Assets/Assets.h"
#include "Function/Graphics/Graphics.h"
#include "Function/Time/Time.h"
#include "Function/Window/Cursor.h"
#include "Function/Window/Input.h"
#include "Function/Window/Screen.h"
#include "Platform/GUI/GUI.h"

namespace BDXKEngine
{
    void Run(const std::function<void()>& onStart)
    {
        if (std::setlocale(LC_ALL, "zh-CN.UTF-8") == nullptr) // NOLINT(concurrency-mt-unsafe)
            throw std::exception("设置本地化失败");

        //平台层
        Window window{L"BDXKEngine"};
        GL::Initialize(&window);
        GUI::Initialize(&window);
        //功能层
        Assets::Initialize(&window);
        //框架层
        Graphics::Initialize(&window);
        Screen::Initialize(&window);
        Cursor::Initialize(&window);
        Input::Initialize(&window);
        Time::Initialize(&window);
        BehaviorEvent::Initialize(&window);
        RenderEvent::Initialize(&window);

        {
            //正式循环
            onStart();
            window.Show();
        }

        for (const ObjectPtr<GameObject>& gameObject : std::vector{GameObject::GetGameObjects()})
            Object::DestroyImmediate(gameObject);

        // Debug::LogError("系统回收检查");
        // BDXKObject::DebugObjectCount();
        // ObjectPtrBase::PrintRefCountMap();
        // BehaviorManager::DebugHandlersCount();
    }
}
