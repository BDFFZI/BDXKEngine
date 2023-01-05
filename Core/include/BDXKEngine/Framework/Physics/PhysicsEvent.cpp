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

    struct CollisionInfo
    {
        template <typename EventType>
        void SendCollisionMessage(std::function<void(EventType* eventHandler, const Collision& collision)> send)
        {
            const ObjectPtr gameObjectA = Object::FindObjectOfInstanceID<GameObject>(AInstanceID);
            const ObjectPtr gameObjectB = Object::FindObjectOfInstanceID<GameObject>(BInstanceID);

            if (gameObjectA.IsNotNull() && gameObjectB.IsNotNull())
            {
                const std::vector<ObjectPtr<Component>> eventHandlers = gameObjectA->GetComponents<EventType, Component>();
                for (auto& component : eventHandlers)
                    if (component.IsNotNull())
                        send(component.ToObject<EventType>(), ACollision);
            }

            if (gameObjectA.IsNotNull() && gameObjectB.IsNotNull())
            {
                const std::vector<ObjectPtr<Component>> eventHandlers = gameObjectB->GetComponents<EventType, Component>();
                for (auto& component : eventHandlers)
                    if (component.IsNotNull())
                        send(component.ToObject<EventType>(), BCollision);
            }
        }

        int AInstanceID;
        Collision ACollision;
        int BInstanceID;
        Collision BCollision;
        long long hash;

        CollisionInfo(int instanceIDA, const Collision& collisionA, int instanceIDB, const Collision& collisionB)
        {
            this->AInstanceID = instanceIDA;
            this->ACollision = collisionA;
            this->BInstanceID = instanceIDB;
            this->BCollision = collisionB;
            hash = static_cast<long long>(instanceIDA) << 32 | instanceIDB;
        }

        CollisionInfo(): AInstanceID(0), BInstanceID(0), hash(0)
        {
        }

        size_t operator()(const CollisionInfo& collisionInfo) const
        {
            return collisionInfo.hash;
        }

        bool operator()(const CollisionInfo& collisionInfoA, const CollisionInfo& collisionInfoB) const
        {
            return collisionInfoA.hash == collisionInfoB.hash;
        }
    };

    std::unordered_map<CollisionInfo, int, CollisionInfo, CollisionInfo> collisionInfos = {};


    class PhysicsCallback : public PhysCallback
    {
        void onContactModify(physx::PxContactModifyPair* const pairs, physx::PxU32 count) override
        {
            const Collision collisionA = {static_cast<Rigidbody*>(pairs->actor[1]->userData), static_cast<Collider*>(pairs->shape[1]->userData)};
            const Collision collisionB = {static_cast<Rigidbody*>(pairs->actor[0]->userData), static_cast<Collider*>(pairs->shape[0]->userData)};
            const CollisionInfo collisionInfo = {
                collisionB.rigidbody->GetGameObject()->GetInstanceID(), collisionA,
                collisionA.rigidbody->GetGameObject()->GetInstanceID(), collisionB,
            };
            if (collisionInfos.contains(collisionInfo) == false)collisionInfos[collisionInfo] = 3;
            else collisionInfos[collisionInfo] = 2;
        }
    };

    void PhysicsEvent::Initialize()
    {
        Window::AddRenewEvent([]
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
                for (std::pair<const CollisionInfo, int>& item : collisionInfos)
                {
                    CollisionInfo collisionInfo = item.first;
                    if (item.second == 3) //首次出现
                    {
                        collisionInfo.SendCollisionMessage<CollisionEnterHandler>([](CollisionEnterHandler* handler, const Collision& collision)
                        {
                            handler->OnCollisionEnter(collision);
                        });

                        item.second = 1;
                    }
                    else if (item.second == 2) //持续出现
                    {
                        collisionInfo.SendCollisionMessage<CollisionStayHandler>([](CollisionStayHandler* handler, const Collision& collision)
                        {
                            handler->OnCollisionStay(collision);
                        });

                        item.second = 1;
                    }
                    else if (item.second == 1) //不再出现
                    {
                        collisionInfo.SendCollisionMessage<CollisionExitHandler>([](CollisionExitHandler* handler, const Collision& collision)
                        {
                            handler->OnCollisionExit(collision);
                        });

                        item.second = -1;
                    }
                }
                std::erase_if(collisionInfos, [](const std::pair<const CollisionInfo, int>& item) { return item.second == -1; });

                //物理更新回调
                for (const auto& drawGUIHandler : fixedUpdateHandlers)
                    if (drawGUIHandler.IsNotNull() && drawGUIHandler->IsNotResource())
                        drawGUIHandler.ToObject<FixedUpdateHandler>()->OnFixedUpdate();
            }
        });
        Phys::SetCallback(new PhysicsCallback());
    }
}
