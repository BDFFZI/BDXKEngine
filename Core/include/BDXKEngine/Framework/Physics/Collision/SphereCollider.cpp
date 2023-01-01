#include "SphereCollider.h"

namespace BDXKEngine
{
    physx::PxGeometry& SphereCollider::GetPxGeometry()
    {
        geometry = physx::PxSphereGeometry{radius};
        return geometry;
    }
    void SphereCollider::Transfer(Transferer& transferer)
    {
        Collider::Transfer(transferer);

        TransferFieldInfo(radius);
    }
}
