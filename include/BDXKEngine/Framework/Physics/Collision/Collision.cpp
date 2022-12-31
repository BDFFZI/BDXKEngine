#include "Collision.h"

#include "BDXKEngine/Framework/Physics/Rigidbody/Rigidbody.h"

namespace BDXKEngine
{
    void Collision::OnEnable()
    {
        rightbody->GetPxRigidbody().attachShape(*shape);
    }
    void Collision::OnDisable()
    {
        rightbody->GetPxRigidbody().detachShape(*shape);
    }
    void Collision::Awake()
    {
        Behavior::Awake();

        material = Phys::GetPhysics().createMaterial(0, 0, 1);
        shape = Phys::GetPhysics().createShape(GetPxGeometry(), *material);

        rightbody = GetGameObject()->GetComponent<Rigidbody>();
        if (rightbody == nullptr)
        {
            rightbody = Create<Rigidbody>(GetGameObject());
            rightbody->SetConstraints(RigidbodyConstraints::FreezeAll);
        }
    }
    void Collision::Destroy()
    {
        shape->release();
        material->release();

        Behavior::Destroy();
    }
}
