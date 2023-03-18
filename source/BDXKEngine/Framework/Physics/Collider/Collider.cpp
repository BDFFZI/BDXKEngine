#include "Collider.h"

#include "BDXKEngine/Framework/Physics/Rigidbody/Rigidbody.h"

namespace BDXKEngine
{
    void Collider::OnFixedUpdate()
    {
        const ObjectPtr<GameObject> rigidbodyGameObject = rigidbody->GetGameObject();
        const ObjectPtr<GameObject> localGameObject = GetGameObject();
        const Matrix4x4 localToRigidbodyMatrix = rigidbodyGameObject->GetWorldToLocalMatrix() * localGameObject->GetLocalToWorldMatrix();

        Vector3 position;
        Vector3 rotation;
        Vector3 scale;
        Matrix4x4::DecomposeTRS(localToRigidbodyMatrix, position, rotation, scale);

        rigidbody->GetPxRigidbody().detachShape(*shape);
        shape->setLocalPose(Phys::CreatePxTransform(position, rotation));
        rigidbody->GetPxRigidbody().attachShape(*shape);
    }
    void Collider::OnEnable()
    {
        shape->setGeometry(GetPxGeometry());
        shape->setSimulationFilterData({static_cast<unsigned int>(GetGameObject()->GetLayer()), 0, 0, 0});
        rigidbody->GetPxRigidbody().attachShape(*shape);
        rigidbody->ResetCenterOfMassAndInertiaTensor();
    }
    void Collider::OnDisable()
    {
        if (rigidbody.IsNotNull()) //假如是删除时触发可能出现rightbody已被删除的情况
        {
            rigidbody->GetPxRigidbody().detachShape(*shape);
            rigidbody->ResetCenterOfMassAndInertiaTensor();
        }
    }
    void Collider::OnAwake()
    {
        shape = Phys::GetPhysics().createShape(GetPxGeometry(), material.IsNotNull() ? material->GetPxMaterial() : Phys::GetMaterial());
        shape->userData = this;
    }
    void Collider::OnDestroy()
    {
        shape->release();
    }

    void Collider::Transfer(Transferer& transferer)
    {
        Behavior::Transfer(transferer);

        TransferFieldInfo(material);
    }
    void Collider::Awake()
    {
        Behavior::Awake();

        rigidbody = GetGameObject()->GetComponentInParent<Rigidbody>();
        if (rigidbody == nullptr)
        {
            rigidbody = Create<Rigidbody>(GetGameObject());
            rigidbody->SetIsKinematic(true);
        }
    }
}
