#pragma once
#include "BDXKEngine/Base/Object/Pointer/ObjectPtr.h"
#include "BDXKEngine/Platform/Window/Window.h"
#include "BDXKEngine/Framework/Physics/Collider/Collider.h"
#include "BDXKEngine/Framework/Physics/Rigidbody/Rigidbody.h"

namespace BDXKEngine
{
    class PhysicsEvent;
    class PhysicsCallback;

    struct Collision
    {
        ObjectPtr<Rigidbody> rigidbody;
        ObjectPtr<Collider> collider;
    };

    class CollisionEnterHandler
    {
        friend PhysicsCallback;
        friend PhysicsEvent;
    protected:
        virtual ~CollisionEnterHandler() = default;
        virtual void OnCollisionEnter(const Collision& collision) = 0;
    };

    class CollisionStayHandler
    {
        friend PhysicsCallback;
        friend PhysicsEvent;
    protected:
        virtual ~CollisionStayHandler() = default;
        virtual void OnCollisionStay(const Collision& collision) = 0;
    };

    class CollisionExitHandler
    {
        friend PhysicsCallback;
        friend PhysicsEvent;
    protected:
        virtual ~CollisionExitHandler() = default;
        virtual void OnCollisionExit(const Collision& collision) = 0;
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
