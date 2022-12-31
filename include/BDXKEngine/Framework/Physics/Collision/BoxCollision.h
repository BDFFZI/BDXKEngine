#pragma once
#include "Collision.h"

namespace BDXKEngine
{
    class BoxCollision : public Collision
    {
        physx::PxBoxGeometry geometry = {};
        
        physx::PxGeometry& GetPxGeometry() override;
    };

    CustomReflection(BoxCollision)
}
