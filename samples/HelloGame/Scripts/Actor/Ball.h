#pragma once
#include "BDXKEngine/Framework/Behavior/Core/Behavior.h"
#include "BDXKEngine/Framework/Physics/PhysicsEvent.h"
#include "BDXKEngine/Framework/Physics/Rigidbody/Rigidbody.h"

namespace HelloGame
{
    using namespace BDXKEngine;

    class Ball : public Behavior, public StartHandler, public DestroyHandler, public FixedUpdateHandler
    {
    public:
        const std::function<void()>& GetDestroyCallback() const;
        void SetDestroyCallback(const std::function<void()>& destroyCallback);
    private:
        float speed = 5;
        void Transfer(Transferer& transferer) override;

        ObjectPtr<Rigidbody> rightbody;
        std::function<void()> destroyCallback;

        void OnStart() override;
        void OnFixedUpdate() override;
        void OnDestroy() override;
    };

    CustomReflection(Ball)
}
