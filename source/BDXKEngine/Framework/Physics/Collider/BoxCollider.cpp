#include "BoxCollider.h"

namespace BDXKEngine
{
    physx::PxGeometry& BoxCollider::GetPxGeometry()
    {
        Vector3 halfSize = GetGameObject()->GetScale();
        halfSize.x *= size.x;
        halfSize.y *= size.y;
        halfSize.z *= size.z;
        halfSize /= 2;

        geometry = physx::PxBoxGeometry(halfSize.x, halfSize.y, halfSize.z);
        return geometry;
    }

    void BoxCollider::Transfer(Transferer& transferer)
    {
        Collider::Transfer(transferer);

        TransferFieldInfo(size);
    }
}
