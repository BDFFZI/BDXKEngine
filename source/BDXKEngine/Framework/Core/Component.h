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
        static ObjectPtr<Component> Create(const ObjectPtr<GameObject>& gameObject, const Type& type);
        template <typename TComponent>
        static ObjectPtr<TComponent> Create(const ObjectPtr<GameObject>& gameObject)
        {
            return Create(gameObject, GetTypeOf<TComponent>()).ToObjectPtr<TComponent>();
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
