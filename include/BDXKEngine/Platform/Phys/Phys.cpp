#include "Phys.h"
#include <iostream>

namespace BDXKEngine
{
    physx::PxPhysics* Phys::physics = nullptr;
    physx::PxScene* Phys::scene = nullptr;

    void Phys::Initialize(Window* window)
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
        sceneDesc.filterShader = PxDefaultSimulationFilterShader;
        scene = physics->createScene(sceneDesc);
    }

    physx::PxPhysics& Phys::GetPhysics()
    {
        return *physics;
    }
    physx::PxScene& Phys::GetScene()
    {
        return *scene;
    }
    Vector3 Phys::ToVector3(const physx::PxVec3T<float>& value)
    {
        return {value.x, value.y, value.z};
    }
    physx::PxVec3T<float> Phys::ToVec3T(Vector3 value)
    {
        return {value.x, value.y, value.z};
    }
}
