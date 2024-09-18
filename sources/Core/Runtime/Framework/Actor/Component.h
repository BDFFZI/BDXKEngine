#pragma once
#include "Core/Runtime/Base/Reflection/Reflection.h"
#include "GameObject.h"
#include "SwitchableObject.h"

namespace BDXKEngine
{
    class Component : public SwitchableObject
    {
    public:
        static ObjectPtr<Component> Create(const ObjectPtr<GameObject>& gameObject, const Type& type, bool instantiate = true);
        template <typename TComponent>
        static ObjectPtr<TComponent> Create(const ObjectPtr<GameObject>& gameObject, const bool instantiate = true)
        {
            TComponent* component = new TComponent();
            Component* componentbase = dynamic_cast<Component*>(component);
            if (componentbase == nullptr)
            {
                Debug::LogError("TComponent并不是组件类型");
                delete component;
                return nullptr;
            }

            componentbase->gameObject = gameObject;
            gameObject->AddComponent(componentbase);

            if (ObjectSerialization::IsPersistent(gameObject))
                ObjectSerialization::MarkPersistent(component, ObjectSerialization::GetPersistent(gameObject));

            if (instantiate)
                AwakeObject(component);

            return component;
        }
        template <class TTarget, class TBack = ObjectPtr<TTarget>>
        static std::vector<TBack> FindComponentsOfType()
        {
            std::vector<TBack> result = {};
            for (const auto& item : allComponentsNotResource)
            {
                TTarget* object = dynamic_cast<TTarget*>(item);
                if (object != nullptr)result.emplace_back(object);
            }

            return result;
        }

        bool GetIsActivating() const override;
        ObjectPtr<GameObject> GetGameObject() const;

        void Transfer(Transferer& transferer) override;

    protected:
        void Enable() override;
        void Disable() override;
        void Destroy() override;

    private:
        inline static std::unordered_set<Component*> allComponentsNotResource = {};

        ObjectPtr<GameObject> gameObject = nullptr;
    };

    CustomReflection(GameObject)
    CustomReflection(Component)
}
