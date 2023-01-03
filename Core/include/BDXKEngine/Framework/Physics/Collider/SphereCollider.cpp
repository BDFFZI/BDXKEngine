#include "SphereCollider.h"
#include <cmath>

namespace BDXKEngine
{
    physx::PxGeometry& SphereCollider::GetPxGeometry()
    {
        const Vector3 scale = GetGameObject()->GetLocalScale();
        geometry = physx::PxSphereGeometry{radius * fmaxf(scale.z, fmaxf(scale.x, scale.y))};
        return geometry;
    }
    void SphereCollider::Transfer(Transferer& transferer)
    {
        Collider::Transfer(transferer);

        TransferFieldInfo(radius);
    }
}
