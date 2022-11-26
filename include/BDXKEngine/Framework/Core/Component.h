#pragma once
#include "BDXKEngine/Base/Reflection/Reflection.h"
#include "GameObject.h"
#include "ScriptableObject.h"

namespace BDXKEngine
{
    class GameObject;

    class Component : public ScriptableObject
    {
    public:
        template <typename TComponent>
        static ObjectPtr<TComponent> Create(const ObjectPtr<GameObject> gameObject)
        {
            ObjectPtr component = new TComponent();
            if (component.template ToObject<Component>() == nullptr)
                throw std::exception("TComponent并不是组件类型");

            component->gameObject = gameObject;
            gameObject->AddComponent(component.template ToObjectPtr<Component>());
            Instantiate(component);

            return component;
        }

        bool GetIsActivating() const override;
        ObjectPtr<GameObject> GetGameObject() const;

        void Transfer(Transferer& transferer) override;
    protected:
        void Destroy() override;
    private:
        ObjectPtr<GameObject> gameObject = nullptr;
    };

    CustomReflection(GameObject)
    CustomReflection(Component)
}
