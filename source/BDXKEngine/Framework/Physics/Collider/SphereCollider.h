#pragma once
#include "Collider.h"

namespace BDXKEngine
{
    class SphereCollider : public Collider
    {
        float radius = 0.5f;

        physx::PxSphereGeometry geometry;

        physx::PxGeometry& GetPxGeometry() override;

        void Transfer(Transferer& transferer) override;
    };

    CustomReflection(SphereCollider)
}
