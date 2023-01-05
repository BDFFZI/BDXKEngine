#include "BDXKEngine.h"
#include <ranges>
#include "Framework/Scene.h"
#include "Framework/Behavior/BehaviorEvent.h"
#include "Framework/Physics/PhysicsEvent.h"
#include "Framework/Renderer/RenderEvent.h"
#include "Function/Graphics/Graphics.h"
#include "Function/Resources/ResourcesDefault.h"
#include "Function/Time/Time.h"
#include "Function/Window/Cursor.h"
#include "Function/Window/Input.h"
#include "Platform/GUI/GUI.h"
#include "Platform/Phys/Phys.h"
#include "Function/Resources/Resources.h"

namespace BDXKEngine
{
    void Run(const std::string& sceneName)
    {
        Run([&]
        {
            Scene::Load(sceneName);
        });
    }
    void Run(const std::function<void()>& onStart)
    {
        if (std::setlocale(LC_ALL, "zh-CN.UTF-8") == nullptr) // NOLINT(concurrency-mt-unsafe)
            throw std::exception("设置本地化失败");

        //平台层
        Window::Initialize(L"BDXKEngine");
        GL::Initialize();
        GUI::Initialize();
        Phys::Initialize();
        //功能层
        Graphics::Initialize();
        Input::Initialize();
        Time::Initialize();
        ResourcesDefault::Initialize();
        //框架层
        PhysicsEvent::Initialize();
        BehaviorEvent::Initialize();
        RenderEvent::Initialize();

        //正式开始
        {
            onStart();
            Window::Show();
        }

        std::vector<ObjectPtrBase> objectPtrBases = {};
        for (const auto& item : Object::GetObjects() | std::ranges::views::values)
            objectPtrBases.emplace_back(item);
        for (const auto& item : objectPtrBases)
            Object::DestroyImmediate(item);

        // Debug::LogError("系统回收检查");
        // BDXKObject::DebugObjectCount();
        // ObjectPtrBase::PrintRefCountMap();
        // BehaviorManager::DebugHandlersCount();
    }
}
