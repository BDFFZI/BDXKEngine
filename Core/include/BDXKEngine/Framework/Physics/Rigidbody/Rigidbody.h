#pragma once
#include "BDXKEngine/Platform/Phys/Phys.h"
#include "BDXKEngine/Framework/Behavior/Core/Behavior.h"
#include "BDXKEngine/Framework/Core/Component.h"

namespace BDXKEngine
{
    class PhysicsEvent;

    class FixedUpdateHandler
    {
        friend PhysicsEvent;
    protected:
        virtual ~FixedUpdateHandler() = default;
        virtual void OnFixedUpdate() = 0;
    };

    enum class RigidbodyConstraints
    {
        None = 0,
        FreezePositionX = physx::PxRigidDynamicLockFlag::eLOCK_LINEAR_X,
        FreezePositionY = physx::PxRigidDynamicLockFlag::eLOCK_LINEAR_Y,
        FreezePositionZ = physx::PxRigidDynamicLockFlag::eLOCK_LINEAR_Z,
        FreezeRotationX = physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_X,
        FreezeRotationY = physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_Y,
        FreezeRotationZ = physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_Z,
        FreezePosition = FreezePositionX | FreezePositionY | FreezePositionZ,
        FreezeRotation = FreezeRotationX | FreezeRotationY | FreezeRotationZ,
        FreezeAll = FreezePositionX | FreezePositionY | FreezePositionZ | FreezeRotationX | FreezeRotationY | FreezeRotationZ,
    };

    inline RigidbodyConstraints operator&(RigidbodyConstraints a, RigidbodyConstraints b)
    {
        return static_cast<RigidbodyConstraints>(static_cast<int>(a) & static_cast<int>(b));
    }
    inline RigidbodyConstraints operator|(RigidbodyConstraints a, RigidbodyConstraints b)
    {
        return static_cast<RigidbodyConstraints>(static_cast<int>(a) | static_cast<int>(b));
    }

    enum class ForceMode
    {
        Force = physx::PxForceMode::eFORCE,
        Acceleration = physx::PxForceMode::eACCELERATION,
        Impulse = physx::PxForceMode::eIMPULSE,
        VelocityChange = physx::PxForceMode::eVELOCITY_CHANGE,
    };

    class Rigidbody : public Behavior, public EnableHandler, public DisableHandler, public UpdateHandler, public FixedUpdateHandler
    {
    public:
        physx::PxRigidDynamic& GetPxRigidbody() const;

        float GetMass() const;
        const Vector3& GetCenterOfMass() const;
        bool GetUseGravity() const;
        bool GetIsKinematic() const;
        RigidbodyConstraints GetConstraints() const;
        void SetMass(float mass);
        void SetCenterOfMass(const Vector3& centerOfMass);
        void SetUseGravity(bool useGravity);
        void SetIsKinematic(bool isKinematic);
        void SetConstraints(RigidbodyConstraints constraints);

        void AddForce(Vector3 force, ForceMode forceMode = ForceMode::Force) const;
        void ResetCenterOfMassAndInertiaTensor() const;
    private:
        float mass = 1;
        Vector3 centerOfMass = Vector3::zero;
        bool useGravity = true;
        bool isKinematic = false;
        RigidbodyConstraints constraints = RigidbodyConstraints::None;
        physx::PxRigidDynamic* actor = nullptr;

        void OnUpdate() override;
        void OnFixedUpdate() override;
        void OnEnable() override;
        void OnDisable() override;

        void Transfer(Transferer& transferer) override;
        void Awake() override;
        void Destroy() override;
    };

    CustomReflection(Rigidbody)
}
