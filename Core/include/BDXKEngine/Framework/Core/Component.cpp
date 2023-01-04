#include "Component.h"

namespace BDXKEngine
{
    ObjectPtr<Component> Component::Create(const ObjectPtr<GameObject>& gameObject, const Type& type)
    {
        ObjectPtr component = Reflection::GetReflection(type).GetConstruction<Component>();
        if (component.IsNull())throw std::exception("TComponent并不是组件类型");

        component->gameObject = gameObject;
        gameObject->AddComponent(component.template ToObjectPtr<Component>());
        Instantiate(component);

        return component;
    }
    bool Component::GetIsActivating() const
    {
        return gameObject->IsActivatingAndEnabling();
    }
    ObjectPtr<GameObject> Component::GetGameObject() const { return gameObject; }

    void Component::Transfer(Transferer& transferer)
    {
        ScriptableObject::Transfer(transferer);

        TransferFieldInfo(gameObject);
    }
    void Component::Destroy()
    {
        gameObject->RemoveComponent(this);

        ScriptableObject::Destroy();
    }
}
