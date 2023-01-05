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
#include "BDXKEngine/Platform/Window/Window.h"

namespace BDXKEngine
{
    class PhysCallback : public physx::PxContactModifyCallback
    {
    };

    class Phys
    {
    public:
        static void Initialize();

        static physx::PxPhysics& GetPhysics();
        static physx::PxScene& GetScene();
        static physx::PxMaterial& GetMaterial();
        static void SetCallback(PhysCallback* physCallback);

        static Vector3 ToVector3(const physx::PxVec3T<float>& value);
        static physx::PxVec3T<float> ToVec3T(Vector3 value);
        static Vector3 ToEulerAngles(const physx::PxQuat& value);
        static physx::PxQuat ToQuat(Vector3 eulerAngles);
    private:
        static physx::PxPhysics* physics;
        static physx::PxScene* scene;
        static physx::PxMaterial* material;
    };
}
