#pragma once
#include "BDXKEngine/Framework/Core/Singleton.h"
#include "BDXKEngine/Platform/Phys/Phys.h"

namespace BDXKEngine
{
    class PhysicsSettings : public Singleton<PhysicsSettings>
    {
    public:
        PhysicsSettings();

        static bool GetLayerCollision(int layerA, int layerB);
        static void SetLayerCollision(int layerA, int layerB, bool value);
    private:
        LayerCollisionMatrix layerCollisionMatrix = {};

        void Transfer(Transferer& transferer) override;
        void Awake() override;
    };

    CustomReflection(PhysicsSettings)
}
