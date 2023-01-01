#include "Phys.h"
#include "BDXKEngine/Base/Data/Mathematics/Matrix/Matrix4x4.h"
#include "BDXKEngine/Base/Data/Mathematics/Quaternion/Quaternion.h"

namespace BDXKEngine
{
    physx::PxPhysics* Phys::physics = nullptr;
    physx::PxScene* Phys::scene = nullptr;
    physx::PxMaterial* Phys::material = nullptr;

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
        //创建默认资源
        material = physics->createMaterial(0.6f, 0.6f, 0);
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
