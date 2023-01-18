#pragma once
#include "BDXKEngine/Framework/Physics/PhysicsEvent.h"
#include "BDXKEngine/Function/Graphics/Material.h"

namespace HelloGame
{
    using namespace BDXKEngine;

    class Gate : public Behavior, public AwakeHandler, public CollisionEnterHandler
    {
        float maxHealthPoint = 100;
        void Transfer(Transferer& transferer) override;

        ObjectPtr<Material> material;
        float healthPoint = 0;
        Color originalColor;

        void OnCollisionEnter(const Collision& collision) override;
        void OnAwake() override;
    };

    CustomReflection(Gate)
}
