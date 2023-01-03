#include "PhysicsEvent.h"
#include "BDXKEngine/Base/Object/Object.h"
#include "BDXKEngine/Framework/Core/Component.h"
#include "BDXKEngine/Framework/Core/ScriptableObject.h"
#include "BDXKEngine/Function/Time/Time.h"
#include "BDXKEngine/Platform/Phys/Phys.h"

namespace BDXKEngine
{
    float PhysicsEvent::fixedTime = 0.02f;
    float PhysicsEvent::fixedTimeDelta = 0;

    std::unordered_map<int, int> collisions = {};

    class PhysicsCallback : public PhysCallback
    {
        void onContactModify(physx::PxContactModifyPair* const pairs, physx::PxU32 count) override
        {
            const int instanceIDA = static_cast<Component*>(pairs->actor[0]->userData)->GetGameObject()->GetInstanceID();
            if (collisions.contains(instanceIDA) == false)collisions[instanceIDA] = 3;
            else collisions[instanceIDA] = 2;

            const int instanceIDB = static_cast<Component*>(pairs->actor[1]->userData)->GetGameObject()->GetInstanceID();
            if (collisions.contains(instanceIDB) == false)collisions[instanceIDB] = 3;
            else collisions[instanceIDB] = 2;
        }
    };

    void PhysicsEvent::Initialize(Window* window)
    {
        window->AddRenewEvent([]
        {
            const std::vector<ObjectPtr<ScriptableObject>> fixedUpdateHandlers =
                ScriptableObject::FindScriptableObjectsOfType<FixedUpdateHandler, ScriptableObject>();

            fixedTimeDelta += Time::GetDeltaTime();
            while (fixedTimeDelta > fixedTime)
            {
                Phys::GetScene().simulate(fixedTime);
                Phys::GetScene().fetchResults(true);
                fixedTimeDelta -= fixedTime;

                //碰撞事件判断
                for (std::pair<const int, int>& item : collisions)
                {
                    const GameObject* target = dynamic_cast<GameObject*>(Object::FindObjectOfInstanceID(item.first));
                    if (target == nullptr) //物体已失效
                    {
                        item.second = -1;
                    }
                    else if (item.second == 3) //首次出现
                    {
                        std::vector<ObjectPtr<Component>> collisionEnterHandlers =
                            target->GetComponents<CollisionEnterHandler, Component>();
                        for (auto& component : collisionEnterHandlers)
                            if (component.IsNotNull())component.ToObject<CollisionEnterHandler>()->OnCollisionEnter(nullptr);

                        item.second = 1;
                    }
                    else if (item.second == 2) //持续出现
                    {
                        std::vector<ObjectPtr<Component>> collisionEnterHandlers =
                            target->GetComponents<CollisionStayHandler, Component>();
                        for (auto& component : collisionEnterHandlers)
                            if (component.IsNotNull())component.ToObject<CollisionStayHandler>()->OnCollisionStay(nullptr);

                        item.second = 1;
                    }
                    else if (item.second == 1) //不再出现
                    {
                        std::vector<ObjectPtr<Component>> collisionEnterHandlers =
                            target->GetComponents<CollisionExitHandler, Component>();
                        for (auto& component : collisionEnterHandlers)
                            if (component.IsNotNull())component.ToObject<CollisionExitHandler>()->OnCollisionExit(nullptr);

                        item.second = -1;
                    }
                }
                std::erase_if(collisions, [](const std::pair<const int, int>& item) { return item.second == -1; });

                //物理更新回调
                for (const auto& drawGUIHandler : fixedUpdateHandlers)
                    if (drawGUIHandler.IsNotNull() && drawGUIHandler->IsNotResource())
                        drawGUIHandler.ToObject<FixedUpdateHandler>()->OnFixedUpdate();
            }
        });
        Phys::SetCallback(new PhysicsCallback());
    }
}
