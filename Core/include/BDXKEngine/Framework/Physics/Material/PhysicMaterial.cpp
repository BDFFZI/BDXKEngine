#include "PhysicMaterial.h"

namespace BDXKEngine
{
    physx::PxMaterial& PhysicMaterial::GetPxMaterial() const
    {
        return *material;
    }
    
    float PhysicMaterial::GetDynamicFriction() const
    {
        return dynamicFriction;
    }
    float PhysicMaterial::GetStaticFriction() const
    {
        return staticFriction;
    }
    float PhysicMaterial::GetBounciness() const
    {
        return bounciness;
    }

    void PhysicMaterial::SetDynamicFriction(float dynamicFriction)
    {
        this->dynamicFriction = dynamicFriction;
        material->setDynamicFriction(dynamicFriction);
    }
    void PhysicMaterial::SetStaticFriction(float staticFriction)
    {
        this->staticFriction = staticFriction;
        material->setStaticFriction(staticFriction);
    }
    void PhysicMaterial::SetBounciness(float bounciness)
    {
        this->bounciness = bounciness;
        material->setRestitution(bounciness);
    }

    void PhysicMaterial::Transfer(Transferer& transferer)
    {
        Object::Transfer(transferer);

        TransferFieldInfo(dynamicFriction);
        TransferFieldInfo(staticFriction);
        TransferFieldInfo(bounciness);
    }
    void PhysicMaterial::Awake()
    {
        Object::Awake();

        material = Phys::GetPhysics().createMaterial(staticFriction, dynamicFriction, bounciness);
    }
    void PhysicMaterial::Destroy()
    {
        material->release();

        Object::Destroy();
    }
}
