#include "SphereCollider.h"

namespace BDXKEngine
{
    physx::PxGeometry& SphereCollider::GetPxGeometry()
    {
        const Vector3 scale = GetGameObject()->GetScale();
        geometry = physx::PxSphereGeometry{radius * fmaxf(scale.z, fmaxf(scale.x, scale.y))};
        return geometry;
    }
    void SphereCollider::Transfer(Transferer& transferer)
    {
        Collider::Transfer(transferer);

        TransferFieldInfo(radius);
    }
}
