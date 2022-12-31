#include "BoxCollision.h"

namespace BDXKEngine
{
    physx::PxGeometry& BoxCollision::GetPxGeometry()
    {
        geometry = physx::PxBoxGeometry(0.5f,0.5f,0.5f);
        return geometry;
    }
}
