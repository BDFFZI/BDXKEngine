#pragma once
#include "BDXKEngine/Base/Object/Pointer/ObjectPtr.h"
#include "BDXKEngine/Platform/Window/Window.h"
#include "BDXKEngine/Framework/Physics/Collider/Collider.h"

namespace BDXKEngine
{
    class PhysicsEvent;
    class PhysicsCallback;

    class CollisionEnterHandler
    {
        friend PhysicsCallback;
        friend PhysicsEvent;
    protected:
        virtual ~CollisionEnterHandler() = default;
        virtual void OnCollisionEnter(ObjectPtr<Collider> collider) = 0;
    };

    class CollisionStayHandler
    {
        friend PhysicsCallback;
        friend PhysicsEvent;
    protected:
        virtual ~CollisionStayHandler() = default;
        virtual void OnCollisionStay(ObjectPtr<Collider> collider) = 0;
    };

    class CollisionExitHandler
    {
        friend PhysicsCallback;
        friend PhysicsEvent;
    protected:
        virtual ~CollisionExitHandler() = default;
        virtual void OnCollisionExit(ObjectPtr<Collider> collider) = 0;
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
