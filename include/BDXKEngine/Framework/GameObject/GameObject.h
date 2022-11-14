#pragma once
#include<vector>
#include "BDXKEngine/Base/Object/ObjectPtr.h"
#include "BDXKEngine/Framework/Object/SwitchableObject.h"

namespace BDXKEditor
{
    class GameObjectEditor;
}

namespace BDXKEngine
{
    class Scene;
    class Component;
    class Transform;


    class GameObject : public SwitchableObject, public SwitchableObjectEvent::UpdateHandler
    {
        friend BDXKEditor::GameObjectEditor;
        friend Scene;
        friend Component;
    public:
        template <typename TComponent>
        ObjectPtr<TComponent> GetComponent() const
        {
            for (const ObjectPtr<Component>& component : components)
            {
                ObjectPtr<TComponent> target = component.ToObjectPtr<TComponent>();
                if (target.IsNull() == false)
                    return target;
            }
            return nullptr;
        }
        std::vector<ObjectPtr<Component>> GetComponents() const;
        ObjectPtr<Transform> GetTransform() const;
        bool GetIsActivating() const override;
        const ObjectPtr<Scene>& GetScene() const;


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
    private:
        ObjectPtr<Scene> scene;
        std::vector<ObjectPtr<Component>> components; //当前物体拥有的组件(由Component负责增减)

        void OnUpdate() override;
        void MarkAwake() override;
        void PreDestroy() override;
        void Transfer(transferer& transferer) override;
    };
}
