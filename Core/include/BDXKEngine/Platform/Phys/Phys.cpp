#include "Phys.h"
#include "BDXKEngine/Base/Data/Mathematics/Matrix/Matrix4x4.h"
#include "BDXKEngine/Base/Data/Mathematics/Quaternion/Quaternion.h"
#include "BDXKEngine/Framework/Core/Component.h"

namespace BDXKEngine
{
    physx::PxPhysics* Phys::physics = nullptr;
    physx::PxScene* Phys::scene = nullptr;
    physx::PxMaterial* Phys::material = nullptr;
    int Phys::layerCollisionMatrix[32];

    bool GetLayerCollision(const LayerCollisionMatrix& matrix, int x, int y)
    {
        return x < y
                   ? (matrix[x] & 1 << (32 - y)) != 0
                   : (matrix[y] & 1 << (32 - x)) != 0;
    }
    void SetLayerCollision(LayerCollisionMatrix& matrix, int x, int y, bool value)
    {
        if (value)
        {
            if (x < y)
                matrix[x] |= 1 << (32 - y);
            else
                matrix[y] |= 1 << (32 - x);
        }
        else
        {
            if (x < y)
                matrix[x] &= ~(1 << (32 - y));
            else
                matrix[y] &= ~(1 << (32 - x));
        }
    }
    void Phys::Initialize()
    {
        using namespace physx;

        static PxDefaultErrorCallback gDefaultErrorCallback;
        static PxDefaultAllocator gDefaultAllocatorCallback;

        //核心系统初始化
        PxFoundation* mFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, gDefaultAllocatorCallback, gDefaultErrorCallback);
        if (mFoundation == nullptr)throw std::exception("核心系统初始化");
        //连接可视化调试器
        PxPvd* mPvd = PxCreatePvd(*mFoundation);
        PxPvdTransport* transport = PxDefaultPvdSocketTransportCreate("127.0.0.1", 5425, 10);
        mPvd->connect(*transport, PxPvdInstrumentationFlag::eALL);
        //物理系统初始化
        physics = PxCreatePhysics(PX_PHYSICS_VERSION, *mFoundation, PxTolerancesScale(), true, mPvd);
        if (physics == nullptr)throw std::exception("物理系统初始化");
        //创建场景
        PxSceneDesc sceneDesc(physics->getTolerancesScale());
        sceneDesc.gravity = PxVec3(0.0f, 0.0f, 0.0f);
        sceneDesc.cpuDispatcher = PxDefaultCpuDispatcherCreate(1);
        sceneDesc.filterShader = [](PxFilterObjectAttributes attributes0,
                                    PxFilterData filterData0,
                                    PxFilterObjectAttributes attributes1,
                                    PxFilterData filterData1,
                                    PxPairFlags& pairFlags,
                                    const void* constantBlock,
                                    PxU32 constantBlockSize)
        {
            pairFlags = PxPairFlag::eSOLVE_CONTACT | PxPairFlag::eDETECT_DISCRETE_CONTACT
                | PxPairFlag::eMODIFY_CONTACTS | PxPairFlag::eNOTIFY_TOUCH_FOUND | PxPairFlag::eNOTIFY_TOUCH_LOST;

            if (GetLayerCollision(static_cast<int>(filterData0.word0), static_cast<int>(filterData1.word0)))
                return PxFilterFlags(PxFilterFlag::eDEFAULT);
            else
                return PxFilterFlags(PxFilterFlag::eSUPPRESS);
        };
        scene = physics->createScene(sceneDesc);
        //创建默认资源
        material = physics->createMaterial(0.6f, 0.6f, 0.0f);
        for (int& i : layerCollisionMatrix)i = ~0;
    }

    physx::PxPhysics& Phys::GetPhysics()
    {
        return *physics;
    }
    physx::PxScene& Phys::GetScene()
    {
        return *scene;
    }
    physx::PxMaterial& Phys::GetMaterial()
    {
        return *material;
    }
    bool Phys::GetLayerCollision(int x, int y)
    {
        return BDXKEngine::GetLayerCollision(layerCollisionMatrix, x, y);
    }
    void Phys::SetCallback(PhysCallback* physCallback)
    {
        scene->setSimulationEventCallback(physCallback);
    }
    void Phys::SetLayerCollision(int x, int y, bool value)
    {
        BDXKEngine::SetLayerCollision(layerCollisionMatrix, x, y, value);
    }
    void Phys::SetLayerCollisionMatrix(int matrix[32])
    {
        for (int index = 0; index < 32; index++)
            layerCollisionMatrix[index] = matrix[index];
    }
    physx::PxTransform Phys::CreatePxTransform(Vector3 position, Vector3 rotation)
    {
        return {ToVec3T(position), ToQuat(rotation)};
    }
    Vector3 Phys::ToVector3(const physx::PxVec3T<float>& value)
    {
        return {value.x, value.y, value.z};
    }
    physx::PxVec3T<float> Phys::ToVec3T(Vector3 value)
    {
        return {value.x, value.y, value.z};
    }
    Vector3 Phys::ToEulerAngles(const physx::PxQuat& value)
    {
        const Quaternion quaternion = {value.x, value.y, value.z, value.w};
        Vector3 eulerAngles = {};
        Matrix4x4::DecomposeRotate(Matrix4x4::Rotate(quaternion), eulerAngles);
        return eulerAngles;
    }
    physx::PxQuat Phys::ToQuat(Vector3 eulerAngles)
    {
        Quaternion quaternion = Quaternion::Euler(eulerAngles);
        return {quaternion.x, quaternion.y, quaternion.z, quaternion.w};
    }
}
