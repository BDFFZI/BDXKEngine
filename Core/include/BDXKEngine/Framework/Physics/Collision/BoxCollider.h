#pragma once
#include "Collider.h"

namespace BDXKEngine
{
    class BoxCollider : public Collider
    {
        Vector3 size = {1, 1, 1};

        physx::PxBoxGeometry geometry;

        physx::PxGeometry& GetPxGeometry() override;

        void Transfer(Transferer& transferer) override;
    };

    CustomReflection(BoxCollider)
}
