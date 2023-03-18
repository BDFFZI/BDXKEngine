#pragma once
#include "BDXKEngine/Base/Object/Object.h"
#include "BDXKEngine/Platform/Phys/Phys.h"

namespace BDXKEngine
{
    class PhysicMaterial : public Object
    {
    public:
        physx::PxMaterial& GetPxMaterial() const;

        float GetDynamicFriction() const;
        float GetStaticFriction() const;
        float GetBounciness() const;
        void SetDynamicFriction(float dynamicFriction);
        void SetStaticFriction(float staticFriction);
        void SetBounciness(float bounciness);
    private:
        float dynamicFriction = 0.6f;
        float staticFriction = 0.6f;
        float bounciness = 0;
        physx::PxMaterial* material = nullptr;

        void Transfer(Transferer& transferer) override;
        void Awake() override;
        void Destroy() override;
    };

    CustomReflection(PhysicMaterial)
}
