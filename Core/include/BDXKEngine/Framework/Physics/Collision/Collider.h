#pragma once
#include "BDXKEngine/Platform/Phys/Phys.h"
#include "BDXKEngine/Framework/Behavior/Core/Behavior.h"
#include "BDXKEngine/Framework/Physics/Material/PhysicMaterial.h"
#include "BDXKEngine/Framework/Physics/Rigidbody/Rigidbody.h"

namespace BDXKEngine
{
    class Collider : public Behavior, public AwakeHandler, public DestroyHandler, public EnableHandler, public DisableHandler
    {
    protected:
        virtual physx::PxGeometry& GetPxGeometry() =0;

        void Transfer(Transferer& transferer) override;
    private:
        ObjectPtr<Rigidbody> rightbody = nullptr;
        ObjectPtr<PhysicMaterial> material = nullptr;
        physx::PxShape* shape = nullptr;

        void OnEnable() override;
        void OnDisable() override;
        void OnAwake() override;
        void OnDestroy() override;
        void Awake() override;
    };
}
