#include "Collider.h"

#include "BDXKEngine/Framework/Physics/Rigidbody/Rigidbody.h"

namespace BDXKEngine
{
    void Collider::OnEnable()
    {
        shape->setGeometry(GetPxGeometry());
        
        rightbody->GetPxRigidbody().attachShape(*shape);
        rightbody->ResetCenterOfMassAndInertiaTensor();
    }
    void Collider::OnDisable()
    {
        rightbody->GetPxRigidbody().detachShape(*shape);
        rightbody->ResetCenterOfMassAndInertiaTensor();
    }

    void Collider::Transfer(Transferer& transferer)
    {
        Behavior::Transfer(transferer);

        TransferFieldInfo(material);
    }
    void Collider::Awake()
    {
        Behavior::Awake();

        shape = Phys::GetPhysics().createShape(GetPxGeometry(), material.IsNotNull() ? material->GetPxMaterial() : Phys::GetMaterial());
        rightbody = GetGameObject()->GetComponent<Rigidbody>();

        if (rightbody == nullptr)
        {
            rightbody = Create<Rigidbody>(GetGameObject());
            rightbody->SetConstraints(RigidbodyConstraints::FreezeAll);
        }
    }
    void Collider::Destroy()
    {
        shape->release();

        Behavior::Destroy();
    }
}
