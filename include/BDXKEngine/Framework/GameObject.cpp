#include "GameObject.h"

#include "BDXKEngine/Base/Extension/Vector.h"

namespace BDXKEngine
{
    std::vector<ObjectPtr<GameObject>> GameObject::allGameObjects;

    ObjectPtr<GameObject> GameObject::Create(const std::string& name)
    {
        Transform source = {};
        GameObject gameObject{};
        gameObject.SetName(name);
        gameObject.components.emplace_back(InstantiateNoAwake<Transform>(&source));

        ObjectPtr<GameObject> result = Instantiate(&gameObject);
        return result;
    }

    std::string GameObject::GetName()
    {
        return name;
    }
    std::vector<ObjectPtr<Component>> GameObject::GetComponents()
    {
        return components;
    }
    ObjectPtr<Transform> GameObject::GetTransform()
    {
        return transform;
    }
    bool GameObject::GetIsActivating() const
    {
        if (const ObjectPtr<Transform> parent = transform->GetParent(); parent != nullptr)
            return parent->GetGameObject()->IsActivatingAndEnabling();

        return true;
    }

    void GameObject::SetName(const std::string& name)
    {
        this->name = name;
    }
    void GameObject::SetIsEnabling(bool state)
    {
        if (isEnabling == state)
            return;
        isEnabling = state;

        if (GetIsActivating())
        {
            UpdateActivating();
        }
    }

    ObjectPtr<GameObject> GameObject::Find(const std::string& name)
    {
        const auto& result = std::ranges::find_if(
            allGameObjects,
            [&](const ObjectPtr<GameObject>& gameObject) { return gameObject->GetName() == name; }
        );

        return result == allGameObjects.end() ? nullptr : *result;
    }

    void GameObject::OnUpdateActivating(bool state)
    {
        for (const auto& component : components)
            component->UpdateActivating();

        const int childCount = transform->GetChildCount();
        for (int i = 0; i < childCount; i++)
            transform->GetChild(i)->GetGameObject()->UpdateActivating();
    }

    void GameObject::Transfer(Transferrer& transferrer)
    {
        SwitchableObject::Transfer(transferrer);

        transferrer.TransferField("name", name);
        //组件
        std::vector<ObjectPtrBase> nativeComponents = std::vector<ObjectPtrBase>{components.begin(), components.end()};
        transferrer.TransferField("components", nativeComponents);
        if (transferrer.GetTransferDirection() == TransferDirection::Input)
        {
            const int count = static_cast<int>(nativeComponents.size());
            components.resize(count);
            for (int i = 0; i < count; i++)
            {
                ObjectPtr component = static_cast<Component*>(nativeComponents[i].ToObjectBase());
                // NOLINT(cppcoreguidelines-pro-type-static-cast-downcast)
                component = InstantiateNoAwake(component.ToObject());
                component->gameObject = this;
                components[i] = component;
            }
        }

        if (transferrer.GetTransferDirection() == TransferDirection::Input)
        {
            transform = GetComponent<Transform>();
        }
    }
    void GameObject::Awake()
    {
        Object::Awake();

        allGameObjects.emplace_back(this);
        for (const auto& component : components)
            Object::Awake(component.ToObjectBase());
    }
    void GameObject::Destroy()
    {
        std::vector components = {this->components};
        for (ObjectPtr<Component>& component : components)
            Object::Destroy(component.ToObjectBase());

        Vector::Remove(allGameObjects, {this});

        Object::Destroy();
    }
}
