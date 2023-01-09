#include "PhysicsEvent.h"

#include <iostream>

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

        ID AInstanceID;
        Collision ACollision;
        ID BInstanceID;
        Collision BCollision;
        long long hash;

        CollisionInfo(ID instanceIDA, const Collision& collisionA, ID instanceIDB, const Collision& collisionB)
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

    //1碰撞中，2结束碰撞，3开始碰撞
    std::unordered_map<CollisionInfo, int, CollisionInfo, CollisionInfo> collisionInfos = {};

    class PhysicsCallback : public PhysCallback
    {
        void onContact(const physx::PxContactPairHeader& pairHeader, const physx::PxContactPair* pairs, physx::PxU32 nbPairs) override
        {
            const Collision collisionA = {
                static_cast<Rigidbody*>(pairHeader.actors[1]->userData),
                static_cast<Collider*>(pairs->shapes[1]->userData)
            };
            if (collisionA.rigidbody.IsNull() || collisionA.collider.IsNull())
                return;
            const Collision collisionB = {
                static_cast<Rigidbody*>(pairHeader.actors[0]->userData),
                static_cast<Collider*>(pairs->shapes[0]->userData)
            };
            if (collisionB.rigidbody.IsNull() || collisionB.collider.IsNull())
                return;

            const CollisionInfo collisionInfo = {
                collisionB.rigidbody->GetGameObject()->GetInstanceID(), collisionA,
                collisionA.rigidbody->GetGameObject()->GetInstanceID(), collisionB,
            };
            if (collisionInfos.contains(collisionInfo) == false)collisionInfos[collisionInfo] = 3;
            else collisionInfos[collisionInfo] = 2;

            //std::cout << collisionInfo.AInstanceID << " : " << collisionInfo.BInstanceID << std::endl;
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
                    if (item.second == 1) //碰撞中
                    {
                        collisionInfo.SendCollisionMessage<CollisionStayHandler>([](CollisionStayHandler* handler, const Collision& collision)
                        {
                            handler->OnCollisionStay(collision);
                        });
                    }
                    else if (item.second == 3) //开始碰撞
                    {
                        collisionInfo.SendCollisionMessage<CollisionEnterHandler>([](CollisionEnterHandler* handler, const Collision& collision)
                        {
                            handler->OnCollisionEnter(collision);
                        });

                        item.second = 1;
                        std::cout << 3 << std::endl;
                    }
                    else if (item.second == 2) //结束碰撞
                    {
                        collisionInfo.SendCollisionMessage<CollisionExitHandler>([](CollisionExitHandler* handler, const Collision& collision)
                        {
                            handler->OnCollisionExit(collision);
                        });

                        item.second = 0;
                        std::cout << 2 << std::endl;
                    }
                }
                std::erase_if(collisionInfos, [](const std::pair<const CollisionInfo, int>& item) { return item.second == 0; });

                //物理更新回调
                for (const auto& drawGUIHandler : fixedUpdateHandlers)
                    if (drawGUIHandler.IsNotNull() && drawGUIHandler->IsNotResource())
                        drawGUIHandler.ToObject<FixedUpdateHandler>()->OnFixedUpdate();
            }
        });
        Phys::SetCallback(new PhysicsCallback());
    }
}
