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

    void GameObject::Export(Exporter& exporter)
    {
        Object::Export(exporter);

        exporter.TransferBool({}, isEnabling);
        exporter.TransferInt({}, static_cast<int>(components.size()));
        for (const auto& component : components)
            exporter.TransferObjectPtr({}, component);
    }
    void GameObject::Import(Importer& importer)
    {
        Object::Import(importer);

        isEnabling = importer.TransferBool({});
        const int count = importer.TransferInt({});
        for (int i = 0; i < count; i++)
        {
            Component* component = static_cast<Component*>(InstantiateNoAwake(importer.TransferObjectPtr({}).ToObjectBase()));
            component->gameObject = this;
            components.emplace_back(component);
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
    
    bool GameObject::GetIsActivating() const
    {
        if (transform != nullptr)
            if (const ObjectPtr<Transform> parent = transform->GetParent(); parent != nullptr)
                return parent->GetGameObject()->GetIsActivating();

        return true;
    }
    void GameObject::SetIsEnabling(bool state)
    {
        if (isEnabling == state)
            return;

        if (GetIsActivating())
        {
            if (state == false)
                SetIsActivating(false);
            else
                SetIsActivating(true);
        }

        isEnabling = state;
    }
    void GameObject::SetIsActivating(bool state)
    {
        for (const auto& component : components)
            component->SetIsActivating(state);

        const int childCount = transform->GetChildCount();
        for (int i = 0; i < childCount; i++)
            transform->GetChild(i)->GetGameObject()->SetIsActivating(state);
    }
}
