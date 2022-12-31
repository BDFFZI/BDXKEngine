#include "Rigidbody.h"
#include "BDXKEngine/Platform/Phys/Phys.h"

namespace BDXKEngine
{
    physx::PxRigidDynamic& Rigidbody::GetPxRigidbody() const
    {
        return *actor;
    }

    float Rigidbody::GetMass() const
    {
        return mass;
    }
    bool Rigidbody::GetUseGravity() const
    {
        return useGravity;
    }
    RigidbodyConstraints Rigidbody::GetConstraints() const
    {
        return constraints;
    }
    void Rigidbody::SetMass(float mass)
    {
        this->mass = mass;
        actor->setMass(mass);
    }
    void Rigidbody::SetUseGravity(bool useGravity)
    {
        this->useGravity = useGravity;
    }
    void Rigidbody::SetConstraints(RigidbodyConstraints constraints)
    {
        this->constraints = constraints;
        actor->setRigidDynamicLockFlags(static_cast<physx::PxRigidDynamicLockFlag::Enum>(constraints));
    }

    void Rigidbody::AddForce(Vector3 force, ForceMode forceMode) const
    {
        if ((constraints & RigidbodyConstraints::FreezePositionX) != RigidbodyConstraints::None)force.x = 0;
        if ((constraints & RigidbodyConstraints::FreezePositionY) != RigidbodyConstraints::None)force.y = 0;
        if ((constraints & RigidbodyConstraints::FreezePositionZ) != RigidbodyConstraints::None)force.z = 0;

        actor->addForce(Phys::ToVec3T(force), static_cast<physx::PxForceMode::Enum>(forceMode));
    }

    void Rigidbody::OnUpdate()
    {
        const Vector3 oldPosition = GetGameObject()->GetPosition();
        Vector3 newPosition = Phys::ToVector3(actor->getGlobalPose().p);

        if ((constraints & RigidbodyConstraints::FreezePositionX) != RigidbodyConstraints::None)newPosition.x = oldPosition.x;
        if ((constraints & RigidbodyConstraints::FreezePositionY) != RigidbodyConstraints::None)newPosition.y = oldPosition.y;
        if ((constraints & RigidbodyConstraints::FreezePositionZ) != RigidbodyConstraints::None)newPosition.z = oldPosition.z;

        GetGameObject()->SetPosition(newPosition);
        actor->setGlobalPose(physx::PxTransform(newPosition.x, newPosition.y, newPosition.z));
    }
    void Rigidbody::OnFixedUpdate()
    {
        if (useGravity)AddForce(Vector3{0, -9.8f, 0} * mass);
    }
    void Rigidbody::OnEnable()
    {
        const Vector3 position = GetGameObject()->GetPosition();
        actor->setGlobalPose(physx::PxTransform(position.x, position.y, position.z));
        actor->setMass(mass);
        actor->setRigidDynamicLockFlags(static_cast<physx::PxRigidDynamicLockFlag::Enum>(constraints));

        Phys::GetScene().addActor(*actor);
    }
    void Rigidbody::OnDisable()
    {
        Phys::GetScene().removeActor(*actor);
    }

    void Rigidbody::Transfer(Transferer& transferer)
    {
        Behavior::Transfer(transferer);

        TransferFieldInfoOf(constraints, int);
        TransferFieldInfo(useGravity);
        TransferFieldInfo(mass);
    }
    void Rigidbody::Awake()
    {
        Behavior::Awake();

        actor = Phys::GetPhysics().createRigidDynamic(physx::PxTransform(0, 0, 0));
    }
    void Rigidbody::Destroy()
    {
        actor->release();

        Behavior::Destroy();
    }
}
