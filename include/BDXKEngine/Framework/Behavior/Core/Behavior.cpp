#include "Behavior.h"
#include <iostream>
#include <sstream>

namespace BDXKEngine
{
    std::unordered_map<StartHandler*, bool> Behavior::allStartHandlers = {};
    std::unordered_map<UpdateHandler*, bool> Behavior::allUpdateHandlers = {};
    std::unordered_map<LateUpdateHandler*, bool> Behavior::allLateUpdateHandlers = {};

    void Behavior::DebugHandlersCount()
    {
        std::stringstream stream = {};
        stream << "StartHandler数量:" << std::to_string(allStartHandlers.size()) << std::endl;
        stream << "UpdateHandler数量:" << std::to_string(allUpdateHandlers.size()) << std::endl;
        stream << "LateUpdateHandler数量:" << std::to_string(allLateUpdateHandlers.size());
        std::cout << stream.str() << std::endl;
    }
    void Behavior::Start()
    {
        for (const auto& handler : allStartHandlers)
        {
            if (handler.second == true)
                handler.first->OnStart();
        }
        allStartHandlers.clear();
    }
    void Behavior::Update()
    {
        Start();
        for (const auto& handler : allUpdateHandlers)
        {
            if (handler.second == true)
                handler.first->OnUpdate();
        }

        erase_if(allUpdateHandlers, [](auto& item) { return item.second == false; });
    }
    void Behavior::LateUpdate()
    {
        Start();
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
        if (updateHandler != nullptr) allUpdateHandlers[updateHandler] = false;
        if (lateUpdateHandler != nullptr) allLateUpdateHandlers[lateUpdateHandler] = false;

        Component::Disable();
    }
    void Behavior::Awake()
    {
        startHandler = dynamic_cast<StartHandler*>(this);
        updateHandler = dynamic_cast<UpdateHandler*>(this);
        lateUpdateHandler = dynamic_cast<LateUpdateHandler*>(this);

        Component::Awake();
    }
}
