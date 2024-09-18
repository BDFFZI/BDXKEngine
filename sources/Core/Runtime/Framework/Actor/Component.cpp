#include "Component.h"

namespace BDXKEngine
{
    ObjectPtr<Component> Component::Create(const ObjectPtr<GameObject>& gameObject, const Type& type, const bool instantiate)
    {
        ObjectPtr component = Reflection::GetReflection(type).GetConstruction<Component>();
        if (component.IsNull())throw std::exception("TComponent并不是组件类型");
        component->gameObject = gameObject;
        gameObject->AddComponent(component.ToObjectPtr<Component>());

        if (ObjectSerialization::IsPersistent(gameObject))
            ObjectSerialization::MarkPersistent(component, ObjectSerialization::GetPersistent(gameObject));

        if (instantiate)
            AwakeObject(component);

        return component;
    }
    bool Component::GetIsActivating() const
    {
        return gameObject->IsActivatingAndEnabling();
    }
    ObjectPtr<GameObject> Component::GetGameObject() const { return gameObject; }

    void Component::Transfer(Transferer& transferer)
    {
        SwitchableObject::Transfer(transferer);

        TransferFieldInfo(gameObject);
    }
    void Component::Enable()
    {
        SwitchableObject::Enable();

        if (IsNotResource())
        {
            assert(allComponentsNotResource.contains(this) == false); //事件函数执行异常
            allComponentsNotResource.insert(this);
        }
    }
    void Component::Disable()
    {
        if (IsNotResource())
        {
            assert(allComponentsNotResource.contains(this)); //事件函数执行异常
            allComponentsNotResource.erase(this);
        }

        SwitchableObject::Disable();
    }
    void Component::Destroy()
    {
        gameObject->RemoveComponent(this);

        SwitchableObject::Destroy();
    }
}
