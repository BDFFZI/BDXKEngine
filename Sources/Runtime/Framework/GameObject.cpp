﻿#include "GameObject.h"

namespace BDXKEngine
{
    std::vector<ObjectPtr<GameObject>> GameObject::allGameObjects;

    ObjectPtr<GameObject> GameObject::Create(std::wstring name)
    {
        GameObject gameObject = {};
        ObjectPtr<GameObject> result = Object::Instantiate(&gameObject);
        result->SetName(name);
        return result;
    }

    std::vector<ObjectPtr<Component>> GameObject::GetComponents()
    {
        return components;
    }

    ObjectPtr<Transform> GameObject::GetTransform()
    {
        return GetComponent<Transform>();
    }

    bool GameObject::GetIsActivatingSelf() const
    {
        return isActivating;
    }

    bool GameObject::GetIsActivatingInHierarchy()
    {
        const ObjectPtr<Transform> transform = GetTransform();
        if (const ObjectPtr<Transform> parent = transform->GetParent(); parent != nullptr)
            return parent->GetGameObject()->GetIsActivatingInHierarchy();

        return GetIsActivatingSelf();
    }

    void GameObject::SetIsActivating(bool state)
    {
        if(isActivating == state)
            return;
        
        isActivating = state;
    }

    ObjectPtr<GameObject> GameObject::Find(std::wstring name)
    {
        return *std::find_if(allGameObjects.begin(), allGameObjects.end(), [=](ObjectPtr<GameObject> gameObject)
        {
            return gameObject->GetName() == name;
        });
    }

    void GameObject::Awake()
    {
        Object::Awake();

        AddComponent<Transform>();
        allGameObjects.push_back(this);
    }

    void GameObject::Destroy()
    {
        std::vector<ObjectPtr<Component>> components = this->components;
        for (ObjectPtr<Component>& component : components)
            Object::Destroy(component.ToObjectBase());

        allGameObjects.erase(std::find(
            allGameObjects.begin(),
            allGameObjects.end(),
            ObjectPtr<GameObject>{this}
        ));

        Object::Destroy();
    }
}
