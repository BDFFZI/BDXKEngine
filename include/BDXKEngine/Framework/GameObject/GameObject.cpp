#include "GameObject.h"
#include "BDXKEngine/Base/Extension/Vector.h"
#include "BDXKEngine/Framework/Scene/Scene.h"
#include "BDXKEngine/Framework/Component/Component.h"
#include "BDXKEngine/Framework/Component/Transform.h"

namespace BDXKEngine
{
    std::vector<ObjectPtr<Component>> GameObject::GetComponents() const
    {
        return components;
    }
    ObjectPtr<Transform> GameObject::GetTransform() const
    {
        return GetComponent<Transform>();
    }
    bool GameObject::GetIsActivating() const
    {
        if (const ObjectPtr<Transform> parent = GetTransform()->GetParent(); parent != nullptr)
            return parent->GetGameObject()->IsActivatingAndEnabling();

        return scene->IsActivatingAndEnabling();
    }
    const ObjectPtr<Scene>& GameObject::GetScene() const
    {
        return scene;
    }

    void GameObject::Transfer(transferer& transferer)
    {
        SwitchableObject::Transfer(transferer);

        transferer.TransferField("scene", scene);
        //组件
        std::vector<ObjectPtrBase> nativeComponents = std::vector<ObjectPtrBase>{components.begin(), components.end()};
        transferer.TransferField("components", nativeComponents);
        if (transferer.GetTransferDirection() == TransferDirection::Input)
        {
            const int count = static_cast<int>(nativeComponents.size());
            components.resize(count);
            for (int i = 0; i < count; i++)
            {
                // NOLINT(cppcoreguidelines-pro-type-static-cast-downcast)
                ObjectPtr component = static_cast<Component*>(nativeComponents[i].ToObjectBase());
                component = InstantiateNoAwake(component.ToObject());
                component->gameObject = this;
                components[i] = component;
            }
        }
    }

    void GameObject::OnUpdate()
    {
        for (const auto& component : components)
            component->SetIsActivating();

        const ObjectPtr<Transform> transform = GetTransform();
        const int childCount = transform->GetChildCount();
        for (int i = 0; i < childCount; i++)
            transform->GetChild(i)->GetGameObject()->SetIsActivating();
    }
    void GameObject::MarkAwake()
    {
        SwitchableObject::MarkAwake();

        for (const auto& component : components)
            Object::MarkAwake(component.ToObjectBase());
    }
    void GameObject::PreDestroy()
    {
        SwitchableObject::PreDestroy();

        const std::vector components = {this->components};
        for (const ObjectPtr<Component>& component : components)
            Object::MarkDestroy(component.ToObjectBase());

        Vector::Remove(scene->gameObjects, {this});
    }
}
