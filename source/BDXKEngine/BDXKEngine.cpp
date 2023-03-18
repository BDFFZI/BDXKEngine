#include "BDXKEngine.h"
#include <ranges>
#include "Framework/Scene.h"
#include "Framework/Behavior/BehaviorEvent.h"
#include "Framework/Physics/PhysicsEvent.h"
#include "Framework/Renderer/RenderEvent.h"
#include "Function/Graphics/Graphics.h"
#include "Function/Time/Time.h"
#include "Function/Window/Input.h"
#include "Platform/GUI/GUI.h"
#include "Platform/Phys/Phys.h"
#include "Function/Resources/Resources.h"
#include "Platform/Audio/Core/Audio.h"

namespace BDXKEngine
{
    bool isRunning;
    std::vector<std::function<bool()>> overConditions;

    void Run(const std::string& sceneName)
    {
        engineBeginEvents.emplace_back([=] { Scene::Load(sceneName); }); //此时添加以保证是最后一个执行的启动事件
        Run();
    }
    void Run(const std::function<void()>& onStart)
    {
        engineBeginEvents.emplace_back(onStart); //此时添加以保证是最后一个执行的启动事件
        Run();
    }
    void Run()
    {
        if (std::setlocale(LC_ALL, "zh-CN.UTF-8") == nullptr) // NOLINT(concurrency-mt-unsafe)
            throw std::exception("设置本地化失败");

        Window::Initialize(L"BDXKEngine");
        Window::AddDestroyEvent([] //保证在所有系统还未关闭前启动
        {
            //销毁物体，触发Destroy事件
            std::vector<ObjectPtrBase> objectPtrBases = {};
            for (const auto& item : Object::GetObjects() | std::ranges::views::values)
                objectPtrBases.emplace_back(item);
            for (const auto& item : objectPtrBases)
                Object::DestroyImmediate(item);

            for (auto& item : engineEndEvents)
                item();
        });

        //平台层
        GL::Initialize();
        GUI::Initialize();
        Phys::Initialize();
        Audio::Initialize();
        //功能层
        Graphics::Initialize();
        Input::Initialize();
        Time::Initialize();
        //框架层
        BehaviorEvent::Initialize();
        PhysicsEvent::Initialize();
        RenderEvent::Initialize();

        //正式开始
        {
            for (auto& item : engineBeginEvents)
                item();
            if (Scene::GetCurrentSceneName().empty())
            {
                if (Resources::IsExisting("Main.scene"))
                    Scene::Load("Main.scene");
                else
                    Scene::Create("Main.scene");
            }

            isRunning = true;
            overConditions.emplace_back([&] { return isRunning == false; });
            Window::Show([&]
            {
                return std::ranges::all_of(
                    overConditions,
                    [](const std::function<bool()>& isOver) { return isOver(); }
                );
            });
        }

        // Debug::LogError("系统回收检查");
        // BDXKObject::DebugObjectCount();
        // ObjectPtrBase::PrintRefCountMap();
        // BehaviorManager::DebugHandlersCount();
    }

    void Quit()
    {
        isRunning = false;
    }
    void AddQuitCondition(const std::function<bool()>& isOver)
    {
        overConditions.emplace_back(isOver);
    }
}
