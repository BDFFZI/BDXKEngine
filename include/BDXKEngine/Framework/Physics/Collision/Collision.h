#pragma once
#include "BDXKEngine/Platform/Phys/Phys.h"
#include "BDXKEngine/Framework/Behavior/Core/Behavior.h"
#include "BDXKEngine/Framework/Physics/Rigidbody/Rigidbody.h"

namespace BDXKEngine
{
    class Collision : public Behavior, public EnableHandler, public DisableHandler
    {
    protected:
        virtual physx::PxGeometry& GetPxGeometry() =0;
    private:
        ObjectPtr<Rigidbody> rightbody = nullptr;
        physx::PxMaterial* material = nullptr;
        physx::PxShape* shape = nullptr;

        void OnEnable() override;
        void OnDisable() override;
        void Awake() override;
        void Destroy() override;
    };
}
