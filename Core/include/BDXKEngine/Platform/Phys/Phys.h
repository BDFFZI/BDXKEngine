#pragma once
#define _CISO646_//C++20中这玩意已经弃用，不让它导入
#pragma comment(lib,"PhysX_static_64.lib")
#pragma comment(lib,"PhysXCommon_static_64.lib")
#pragma comment(lib,"PhysXFoundation_static_64.lib")
#pragma comment(lib,"PhysXExtensions_static_64.lib")
#pragma comment(lib,"PhysXPvdSDK_static_64.lib")
#define PX_PHYSX_STATIC_LIB
#include "BDXKEngine/Base/Data/Mathematics/Vector/Vector3.h"
#include "PhysX5/PxPhysicsAPI.h"

namespace BDXKEngine
{
    typedef int LayerCollisionMatrix[32]; //上三角矩阵
    bool GetLayerCollision(const LayerCollisionMatrix& matrix, int x, int y);
    void SetLayerCollision(LayerCollisionMatrix& matrix, int x, int y, bool value);

    class PhysCallback : public physx::PxSimulationEventCallback
    {
    protected:
        void onContact(const physx::PxContactPairHeader& pairHeader, const physx::PxContactPair* pairs, physx::PxU32 nbPairs) override
        {
        }
        void onTrigger(physx::PxTriggerPair* pairs, physx::PxU32 count) override
        {
        }

        void onWake(physx::PxActor** actors, physx::PxU32 count) override
        {
        }
        void onSleep(physx::PxActor** actors, physx::PxU32 count) override
        {
        }

        void onConstraintBreak(physx::PxConstraintInfo* constraints, physx::PxU32 count) override
        {
        }
        void onAdvance(const physx::PxRigidBody* const* bodyBuffer, const physx::PxTransform* poseBuffer, const physx::PxU32 count) override
        {
        }
    };

    class Phys
    {
    public:
        static void Initialize();

        static physx::PxPhysics& GetPhysics();
        static physx::PxScene& GetScene();
        static physx::PxMaterial& GetMaterial();
        static bool GetLayerCollision(int x, int y);

        static void SetCallback(PhysCallback* physCallback);
        static void SetLayerCollision(int x, int y, bool value);
        static void SetLayerCollisionMatrix(int matrix[32]);

        static Vector3 ToVector3(const physx::PxVec3T<float>& value);
        static physx::PxVec3T<float> ToVec3T(Vector3 value);
        static Vector3 ToEulerAngles(const physx::PxQuat& value);
        static physx::PxQuat ToQuat(Vector3 eulerAngles);
    private:
        static physx::PxPhysics* physics;
        static physx::PxScene* scene;
        static physx::PxMaterial* material;
        static LayerCollisionMatrix layerCollisionMatrix;
    };
}
