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
    const Vector3& Rigidbody::GetCenterOfMass() const
    {
        return centerOfMass;
    }
    bool Rigidbody::GetUseGravity() const
    {
        return useGravity;
    }
    bool Rigidbody::IsIsKinematic() const
    {
        return isKinematic;
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
    void Rigidbody::SetCenterOfMass(const Vector3& centerOfMass)
    {
        this->centerOfMass = centerOfMass;
        actor->setCMassLocalPose({centerOfMass.x, centerOfMass.y, centerOfMass.z});
    }
    void Rigidbody::SetUseGravity(bool useGravity)
    {
        this->useGravity = useGravity;
    }
    void Rigidbody::SetIsKinematic(bool isKinematic)
    {
        this->isKinematic = isKinematic;
        actor->setRigidBodyFlag(physx::PxRigidBodyFlag::eKINEMATIC, isKinematic);
    }
    void Rigidbody::SetConstraints(RigidbodyConstraints constraints)
    {
        this->constraints = constraints;
        actor->setRigidDynamicLockFlags(static_cast<physx::PxRigidDynamicLockFlag::Enum>(constraints));
    }

    void Rigidbody::AddForce(Vector3 force, ForceMode forceMode) const
    {
        if (isKinematic)return;

        if ((constraints & RigidbodyConstraints::FreezePositionX) != RigidbodyConstraints::None)force.x = 0;
        if ((constraints & RigidbodyConstraints::FreezePositionY) != RigidbodyConstraints::None)force.y = 0;
        if ((constraints & RigidbodyConstraints::FreezePositionZ) != RigidbodyConstraints::None)force.z = 0;

        if (force != Vector3::zero)
            actor->addForce(Phys::ToVec3T(force), static_cast<physx::PxForceMode::Enum>(forceMode));
    }
    void Rigidbody::ResetCenterOfMassAndInertiaTensor() const
    {
        physx::PxRigidBodyExt::updateMassAndInertia(*actor, 1);
        actor->setMass(mass);
    }

    void Rigidbody::OnUpdate()
    {
        if (isKinematic)
        {
            actor->setGlobalPose({
                Phys::ToVec3T(GetGameObject()->GetPosition()),
                Phys::ToQuat(GetGameObject()->GetEulerAngles())
            });
        }
        else
        {
            const physx::PxTransform newTransform = actor->getGlobalPose();
            GetGameObject()->SetPosition(Phys::ToVector3(newTransform.p));
            GetGameObject()->SetEulerAngles(Phys::ToEulerAngles(newTransform.q));
        }
    }
    void Rigidbody::OnFixedUpdate()
    {
        if (useGravity)
            AddForce(Vector3{0, -9.8f, 0}, ForceMode::Acceleration);
    }
    void Rigidbody::OnEnable()
    {
        ResetCenterOfMassAndInertiaTensor();
        actor->setRigidBodyFlag(physx::PxRigidBodyFlag::eKINEMATIC, isKinematic);
        actor->setRigidDynamicLockFlags(static_cast<physx::PxRigidDynamicLockFlag::Enum>(constraints));
        actor->setGlobalPose(physx::PxTransform(
            Phys::ToVec3T(GetGameObject()->GetPosition()),
            Phys::ToQuat(GetGameObject()->GetEulerAngles())
        ));

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
        TransferFieldInfo(mass);
        TransferFieldInfo(useGravity);
        TransferFieldInfo(isKinematic);
    }
    void Rigidbody::Awake()
    {
        if (GetGameObject()->GetComponents<Rigidbody>().size() > 1)
            throw std::exception("每个物体只能装一个Rigidbody");

        Behavior::Awake();
        
        actor = Phys::GetPhysics().createRigidDynamic(physx::PxTransform(0, 0, 0));
        actor->userData = this;
    }
    void Rigidbody::Destroy()
    {
        actor->release();

        Behavior::Destroy();
    }
}
