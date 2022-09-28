#include "GameObject.h"

#include "Base/Extension/Vector.h"

namespace BDXKEngine
{
    std::vector<ObjectPtr<GameObject>> GameObject::allGameObjects;

    ObjectPtr<GameObject> GameObject::Create(const std::wstring& name)
    {
        Transform source = {};
        GameObject gameObject{};
        gameObject.SetName(name);
        gameObject.components.emplace_back(InstantiateNoAwake<Transform>(&source));

        ObjectPtr<GameObject> result = Instantiate(&gameObject);
        return result;
    }

    std::vector<ObjectPtr<Component>> GameObject::GetComponents()
    {
        return components;
    }
    ObjectPtr<Transform> GameObject::GetTransform()
    {
        return transform;
    }


    ObjectPtr<GameObject> GameObject::Find(const std::wstring& name)
    {
        return *std::ranges::find_if(allGameObjects, [=](const ObjectPtr<GameObject>& gameObject)
        {
            return gameObject->GetName() == name;
        });
    }


    bool GameObject::GetIsActivating() const
    {
        if (const ObjectPtr<Transform> parent = transform->GetParent(); parent != nullptr)
            return parent->GetGameObject()->IsActivatingAndEnabling();

        return true;
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
        ObjectSwitchable::Transfer(transferrer);

        int componentsCount = components.size();
        transferrer.TransferInt(nameof(componentsCount), componentsCount);

        if (transferrer.GetTransferDirection() == TransferDirection::Input)components.resize(componentsCount);
        for (int i = 0; i < componentsCount; i++)
        {
            ObjectPtr<Component>& component = components[i];
            transferrer.TransferObjectPtr(L"component_" + std::to_wstring(i), component);

            if (transferrer.GetTransferDirection() == TransferDirection::Input)
            {
                component = InstantiateNoAwake(component.ToObjectBase());
                component->gameObject = this;
            }
        }

        transform = GetComponent<Transform>();
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
