#pragma once
#include "BDXKEngine/Platform/Window/Window.h"

namespace BDXKEngine
{
    class PhysicsEvent;

    class FixedUpdateHandler
    {
        friend PhysicsEvent;
    protected:
        virtual void OnFixedUpdate() = 0;
        virtual ~FixedUpdateHandler() = default;
    };

    class PhysicsEvent
    {
    public:
        static void Initialize(Window* window);
    
    private:
        static float fixedTime;
        static float fixedTimeDelta;
    };
}
