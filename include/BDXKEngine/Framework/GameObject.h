#pragma once
#include<vector>
#include "BDXKEngine/Base/Object/ObjectPtr.h"
#include "BDXKEngine/Framework/Component/Component.h"
#include "BDXKEngine/Framework/Component/Transform.h"

namespace BDXKEngine
{
    class GameObjectManager;

    class GameObject : public SwitchableObject
    {
        friend Component;
        friend Transform;
        friend GameObjectManager;
    public:
        static ObjectPtr<GameObject> Create(const std::string& name = "New GameObject");

        template <typename TComponent>
        ObjectPtr<TComponent> GetComponent()
        {
            for (ObjectPtr<Component>& component : components)
            {
                ObjectPtr<TComponent> target = component.ToObjectPtr<TComponent>();
                if (target.IsNull() == false)
                    return target;
            }
            return nullptr;
        }
        std::vector<ObjectPtr<Component>> GetComponents();
        ObjectPtr<Transform> GetTransform();
        bool GetIsActivating() const override;
        void SetIsEnabling(bool state) override;


        template <typename TComponent>
        ObjectPtr<TComponent> AddComponent()
        {
            //初始化Component
            TComponent source = {};
            if (dynamic_cast<Component*>(&source) == nullptr)
                Debug::LogException("TComponent}并不是组件类型");
            source.gameObject = this;
            ObjectPtr<TComponent> result = Instantiate<TComponent>(&source);
            components.emplace_back(result.template ToObjectPtr<Component>());
            return result;
        }

        static ObjectPtr<GameObject> Find(const std::string& name);
    private:
        //所有物体(由GameObject负责增减)
        static std::vector<ObjectPtr<GameObject>> allGameObjects;

        //当前物体拥有的组件(由Component负责增减)
        std::vector<ObjectPtr<Component>> components;
        ObjectPtr<Transform> transform;

        void OnUpdateActivating(bool state) override;

        void Transfer(Transferrer& transferrer) override;
        void Awake() override;
        void Destroy() override;
    };
}
