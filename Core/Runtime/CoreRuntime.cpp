#include "CoreRuntime.h"
#include <algorithm>
#include <ranges>
#include "Framework/Actor/Behavior.h"
#include "Framework/Scene/Scene.h"
#include "Function/GUI/GUI.h"
#include "Platform/Window/Window.h"

namespace BDXKEngine
{
    bool needQuit = false;
    std::vector<std::function<bool()>> overConditions;

    void CoreRuntime::Run(const std::function<void()>& onStarted)
    {
        Window::AddStartEvent(onStarted, 20);

        Run();
    }
    void CoreRuntime::Run()
    {
        if (std::setlocale(LC_ALL, "zh-CN.UTF-8") == nullptr) // NOLINT(concurrency-mt-unsafe)
            throw std::exception("设置本地化失败");

        //设置引擎内置事件
        AddQuitCondition([&] { return needQuit; });
        //启动事件
        Window::AddStartEvent([]
        {
            //加载场景
            if (Scene::GetActiveScene().IsNull())
            {
                if (Scene::IsExisting("Main.scene"))
                    Scene::Load("Main.scene");
                else
                    Scene::Create("Main.scene");
            }
        }, 10); //加载场景使用事件的方式，是故意延迟给Editor层，使其有机会预处理
        //更新事件
        Window::AddUpdateEvent([]
        {
            Texture::SetRenderTargetDefault();
            GL::Clear(true, true);
            GUI::BeginDraw();
        }, -100); //帧开始
        Window::AddUpdateEvent(Behavior::RunUpdate, 0);
        Window::AddUpdateEvent(Behavior::RunLateUpdate, 10);
        Window::AddUpdateEvent([]
        {
            GUI::EndDraw();
            GL::Present();
        }, 100); //帧结束

        //内置系统初始化
        Window::Initialize("BDXKEngine");
        GL::Initialize(Window::GetHwnd());
        GUI::Initialize(Window::GetGlfwWindow());
        Texture::ResetDefaultRenderTarget();

        //正式开始
        Window::Run([&]
        {
            return std::ranges::all_of(
                overConditions,
                [](const std::function<bool()>& isOver) { return isOver(); }
            );
        });

        //关闭引擎时，销毁物体，触发Destroy事件
        std::vector<ObjectPtrBase> objectPtrBases = {};
        for (const auto& item : Object::GetAllObjects() | std::ranges::views::values)
            objectPtrBases.emplace_back(item);
        for (const auto& item : objectPtrBases)
            Object::DestroyObject(item);

        // Debug::LogError("系统回收检查");
        // BDXKObject::DebugObjectCount();
        // ObjectPtrBase::PrintRefCountMap();
        // BehaviorManager::DebugHandlersCount();
    }

    void CoreRuntime::AddQuitCondition(const std::function<bool()>& isOver)
    {
        overConditions.emplace_back(isOver);
    }
    void CoreRuntime::Quit()
    {
        needQuit = true;
    }
}
