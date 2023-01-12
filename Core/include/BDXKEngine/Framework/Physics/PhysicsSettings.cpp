#include "PhysicsSettings.h"
#include <format>
#include "BDXKEngine/Platform/Phys/Phys.h"

namespace BDXKEngine
{
    PhysicsSettings::PhysicsSettings()
    {
        for (int& i : layerCollisionMatrix)i = ~0;
    }
    bool PhysicsSettings::GetLayerCollision(int layerA, int layerB)
    {
        return BDXKEngine::GetLayerCollision(GetSingleton()->layerCollisionMatrix, layerA, layerB);
    }
    void PhysicsSettings::SetLayerCollision(int layerA, int layerB, bool value)
    {
        BDXKEngine::SetLayerCollision(GetSingleton()->layerCollisionMatrix, layerA, layerB, value);
        Phys::SetLayerCollision(layerA, layerB, value);
    }
    void PhysicsSettings::Transfer(Transferer& transferer)
    {
        Singleton<PhysicsSettings>::Transfer(transferer);

        for (int x = 0; x < 32; x++)
        {
            std::string row = std::format("LayerCollision_{}", x);
            transferer.PushPath(row);
            for (int y = x; y < 32; y++)
            {
                bool layerCollision = BDXKEngine::GetLayerCollision(layerCollisionMatrix, x, y);
                transferer.TransferFieldOf<bool>(std::to_string(y), layerCollision);
                BDXKEngine::SetLayerCollision(layerCollisionMatrix, x, y, layerCollision);
            }
            transferer.PopPath(row);
        }
    }
    void PhysicsSettings::Awake()
    {
        Singleton<PhysicsSettings>::Awake();

        Phys::SetLayerCollisionMatrix(layerCollisionMatrix);
    }
}
