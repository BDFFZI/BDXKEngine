#include "Behavior.h"
#include <iostream>
#include <sstream>
#include "Core/Runtime/Base/Object/Serialization/ObjectSerialization.h"

namespace BDXKEngine
{
    std::unordered_map<StartEvent*, bool> Behavior::allStartHandlers = {};
    std::unordered_map<UpdateEvent*, bool> Behavior::allUpdateHandlers = {};
    std::unordered_map<LateUpdateEvent*, bool> Behavior::allLateUpdateHandlers = {};

    void Behavior::DebugHandlersCount()
    {
        std::stringstream stream = {};
        stream << "StartHandler数量:" << std::to_string(allStartHandlers.size()) << std::endl;
        stream << "UpdateHandler数量:" << std::to_string(allUpdateHandlers.size()) << std::endl;
        stream << "LateUpdateHandler数量:" << std::to_string(allLateUpdateHandlers.size());
        std::cout << stream.str() << std::endl;
    }
    void Behavior::RunStart()
    {
        for (const auto& handler : allStartHandlers)
        {
            if (handler.second == true)
                handler.first->OnStart();
        }
        allStartHandlers.clear();
    }
    void Behavior::RunUpdate()
    {
        RunStart();
        for (const auto& handler : allUpdateHandlers)
        {
            if (handler.second == true)
                handler.first->OnUpdate();
        }

        erase_if(allUpdateHandlers, [](auto& item) { return item.second == false; });
    }
    void Behavior::RunLateUpdate()
    {
        RunStart();
        for (const auto& handler : allLateUpdateHandlers)
        {
            if (handler.second == true)
                handler.first->OnLateUpdate();
        }

        erase_if(allLateUpdateHandlers, [](auto& item) { return item.second == false; });
    }

    void Behavior::Enable()
    {
        Component::Enable();

        //状态事件
        if (isFirstEnable == false)
        {
            if (const auto handler = dynamic_cast<AwakeEvent*>(this); handler != nullptr)
                if (IsNotResource()) handler->OnAwake();
            isFirstEnable = true;
        }
        if (const auto handler = dynamic_cast<EnableEvent*>(this); handler != nullptr)
            if (IsNotResource()) handler->OnEnable();

        //更新事件
        if (startHandler != nullptr)
        {
            allStartHandlers[startHandler] = true;
            startHandler = nullptr;
        }
        if (updateHandler != nullptr) allUpdateHandlers[updateHandler] = true;
        if (lateUpdateHandler != nullptr) allLateUpdateHandlers[lateUpdateHandler] = true;
    }
    void Behavior::Disable()
    {
        //更新事件
        if (updateHandler != nullptr) allUpdateHandlers[updateHandler] = false;
        if (lateUpdateHandler != nullptr) allLateUpdateHandlers[lateUpdateHandler] = false;

        //状态事件
        if (const auto handler = dynamic_cast<DisableEvent*>(this); handler != nullptr)
            if (IsNotResource()) handler->OnDisable();
        if (GetIsDestroyed())
        {
            if (const auto handler = dynamic_cast<DestroyEvent*>(this); handler != nullptr)
                if (IsNotResource()) handler->OnDestroy();
        }

        Component::Disable();
    }
    void Behavior::Awake()
    {
        if (IsNotResource()) startHandler = dynamic_cast<StartEvent*>(this);
        if (IsNotResource()) updateHandler = dynamic_cast<UpdateEvent*>(this);
        if (IsNotResource()) lateUpdateHandler = dynamic_cast<LateUpdateEvent*>(this);

        Component::Awake();
    }
}
