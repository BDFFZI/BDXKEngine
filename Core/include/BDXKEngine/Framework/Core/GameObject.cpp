#include "GameObject.h"
#include <algorithm>
#include "BDXKEngine/Platform/Serialization/Serialization.h"
#include "Component.h"

namespace BDXKEngine
{
    std::vector<ObjectPtr<GameObject>> GameObject::gameObjects;
    std::vector<ObjectPtr<GameObject>> GameObject::gameObjectsHiding;

    ObjectPtr<GameObject> GameObject::Create(const std::string& name, const ObjectPtr<GameObject>& parent)
    {
        ObjectPtr gameObject = new GameObject();
        Reflection::GetReflection(GetTypeOf<GameObject>()).GetFieldOf<std::string>(gameObject.ToObjectBase(), "name") = name;
        Instantiate(gameObject);
        if (parent.IsNotNull()) gameObject->SetParent(parent, false);
        return gameObject;
    }
    const std::vector<ObjectPtr<GameObject>>& GameObject::GetGameObjects()
    {
        return gameObjects;
    }
    const std::vector<ObjectPtr<GameObject>>& GameObject::GetGameObjectsHiding()
    {
        return gameObjectsHiding;
    }
    ObjectPtr<GameObject> GameObject::Find(const std::string& name)
    {
        const auto result = std::ranges::find_if(gameObjects, [name](auto& item) { return item->GetName() == name; });
        return result == gameObjects.end() ? nullptr : *result;
    }
    void GameObject::Hide(const ObjectPtr<GameObject>& gameObject)
    {
        if (gameObject->GetParent().IsNotNull())
            throw std::exception("只有根物体运行被隐藏");

        if (std::ranges::all_of(gameObjectsHiding, [&](const ObjectPtr<GameObject>& item) { return item != gameObject; }))
        {
            gameObjectsHiding.push_back(gameObject);
            std::erase(gameObjects, gameObject);
        }
    }
    void GameObject::UnHide(const ObjectPtr<GameObject>& gameObject)
    {
        if (std::ranges::all_of(gameObjects, [&](const ObjectPtr<GameObject>& item) { return item != gameObject; }))
        {
            gameObjects.push_back(gameObject);
            std::erase(gameObjectsHiding, gameObject);
        }
    }
    void GameObject::Clear()
    {
        for (const auto& item : std::vector{GetGameObjects()})
            DestroyImmediate(item);
    }
    bool GameObject::GetIsActivating() const
    {
        if (parent != nullptr)
            return parent->IsActivatingAndEnabling();
        return true;
    }

    ObjectPtr<GameObject> GameObject::GetParent()
    {
        return parent;
    }
    ObjectPtr<GameObject> GameObject::GetChild(int index)
    {
        return children[index];
    }

    ObjectPtr<Component> GameObject::GetComponent(const Type& type) const
    {
        for (const ObjectPtr<Component>& component : components)
        {
            if (Reflection::GetReflection(component.ToObjectBase()).CanConvertTo(type))
                return component;
        }
        return nullptr;
    }
    int GameObject::GetChildCount() const
    {
        return static_cast<int>(children.size());
    }
    std::vector<ObjectPtr<Component>> GameObject::GetComponents() const
    {
        return components;
    }

    const std::string& GameObject::GetName() const
    {
        return name;
    }
    int GameObject::GetLayer() const
    {
        return layer;
    }
    Vector3 GameObject::GetPosition() const
    {
        return position;
    }
    Vector3 GameObject::GetEulerAngles() const
    {
        return eulerAngles;
    }
    Vector3 GameObject::GetScale() const
    {
        return scale;
    }
    Vector3 GameObject::GetLocalPosition() const
    {
        return localPosition;
    }
    Vector3 GameObject::GetLocalEulerAngles() const
    {
        return localEulerAngles;
    }
    Vector3 GameObject::GetLocalScale() const
    {
        return localScale;
    }
    Matrix4x4 GameObject::GetLocalToWorldMatrix() const
    {
        return localToWorldMatrix;
    }
    Matrix4x4 GameObject::GetWorldToLocalMatrix() const
    {
        return worldToLocalMatrix;
    }
    Vector3 GameObject::GetRight() const
    {
        return localToWorldMatrix.MultiplyVector(Vector3::right);
    }
    Vector3 GameObject::GetUp() const
    {
        return localToWorldMatrix.MultiplyVector(Vector3::up);
    }
    Vector3 GameObject::GetFront() const
    {
        return localToWorldMatrix.MultiplyVector(Vector3::front);
    }


    void GameObject::SetName(const std::string& name)
    {
        this->name = name;
    }
    void GameObject::SetLayer(int layer)
    {
        this->layer = layer;
    }
    void GameObject::SetParent(const ObjectPtr<GameObject>& newparent, bool worldPositionStays)
    {
        //嵌套检查
        if (newparent.IsNull() == false)
        {
            const ObjectPtr current = this;
            ObjectPtr<GameObject> newUpLayer = newparent;
            do
            {
                if (newUpLayer == current)
                    throw std::exception("你在试图让一个父物体或自身成为其的孩子，这会导致嵌套循环，是不允许的。");
                newUpLayer = newUpLayer->parent;
            }
            while (newUpLayer.IsNull() == false);
        }

        if (parent.IsNull() == false)std::erase(parent->children, this);
        if (newparent.IsNull() == false)newparent->children.emplace_back(this);
        this->parent = newparent;


        const Vector3 oldScale = scale;
        const Vector3 oldEulerAngles = eulerAngles;
        const Vector3 oldPosition = position;

        RenewScale();
        RenewEulerAngles();
        RenewPositionAndMatrix();
        UpdateActivating();

        if (worldPositionStays)
        {
            SetScale(oldScale);
            SetEulerAngles(oldEulerAngles);
            SetPosition(oldPosition);
        }
    }
    void GameObject::SetLocalPosition(Vector3 value)
    {
        localPosition = value;
        RenewPositionAndMatrix();
    }
    void GameObject::SetLocalEulerAngles(Vector3 value)
    {
        value.x = static_cast<float>(std::fmod(value.x, 360));
        value.y = static_cast<float>(std::fmod(value.y, 360));
        value.z = static_cast<float>(std::fmod(value.z, 360));
        localEulerAngles = value;
        RenewEulerAngles();
        RenewPositionAndMatrix();
    }
    void GameObject::SetLocalScale(Vector3 value)
    {
        localScale = value;
        RenewScale();
        RenewPositionAndMatrix();
    }
    void GameObject::SetPosition(Vector3 value)
    {
        if (parent.IsNull()) SetLocalPosition(value);
        else SetLocalPosition(parent->GetWorldToLocalMatrix().MultiplyPoint(value));
    }
    void GameObject::SetEulerAngles(Vector3 value)
    {
        if (parent.IsNull()) SetLocalEulerAngles(value);
        else SetLocalEulerAngles(localEulerAngles + value - eulerAngles);
    }
    void GameObject::SetScale(Vector3 value)
    {
        if (parent.IsNull()) SetLocalScale(value);
        else SetLocalScale(value / scale * localScale);
    }

    void GameObject::RenewPositionAndMatrix(bool renewChild)
    {
        position = parent.IsNull() ? localPosition : parent->GetLocalToWorldMatrix().MultiplyPoint(localPosition);
        const Matrix4x4 parentLocalToWorldMatrix = parent.IsNull() ? Matrix4x4::identity : parent->GetLocalToWorldMatrix();

        localToWorldMatrix = parentLocalToWorldMatrix * Matrix4x4::TRS(localPosition, localEulerAngles, localScale);
        worldToLocalMatrix = localToWorldMatrix.GetInverse();

        if (renewChild)
            for (const ObjectPtr<GameObject>& child : children)
                child->RenewPositionAndMatrix();
    }
    void GameObject::RenewEulerAngles(bool renewChild)
    {
        const Vector3 parentalEulerAngles = parent.IsNull() ? Vector3::zero : parent->GetEulerAngles();

        eulerAngles.x = static_cast<float>(std::fmod(parentalEulerAngles.x + localEulerAngles.x, 360));
        eulerAngles.y = static_cast<float>(std::fmod(parentalEulerAngles.y + localEulerAngles.y, 360));
        eulerAngles.z = static_cast<float>(std::fmod(parentalEulerAngles.z + localEulerAngles.z, 360));

        if (renewChild)
            for (const ObjectPtr<GameObject>& child : children)
                child->RenewEulerAngles();
    }
    void GameObject::RenewScale(bool renewChild)
    {
        const Vector3 parentalScale = parent.IsNull() ? Vector3::one : parent->GetScale();

        scale.x = parentalScale.x * localScale.x;
        scale.y = parentalScale.y * localScale.y;
        scale.z = parentalScale.z * localScale.z;

        if (renewChild)
            for (const ObjectPtr<GameObject>& child : children)
                child->RenewScale();
    }

    void GameObject::AddComponent(const ObjectPtr<Component>& component)
    {
        if (component->GetGameObject() != this)throw std::exception("目标组件未绑定到该物体");

        const auto target = std::ranges::find(components, component);
        if (target != components.end())throw std::exception("对应组件已添加");
        components.emplace_back(component);
    }
    void GameObject::RemoveComponent(const ObjectPtr<Component>& component)
    {
        if (component->GetGameObject() != this)throw std::exception("目标组件未绑定到该物体");

        const auto target = std::ranges::find(components, component);
        if (target == components.end())throw std::exception("未查询到对应组件");
        components.erase(target);
    }

    void GameObject::Transfer(Transferer& transferer)
    {
        ScriptableObject::Transfer(transferer);

        for (ObjectPtr<GameObject>& child : children)
        {
            const Reflection& gameObjectReflection = Reflection::GetReflection(GetTypeOf<GameObject>());
            if (child->GetParent() != this)
            {
                child = Serialization::Clone<GameObject>(child, false);
                gameObjectReflection.GetFieldOf<ObjectPtr<GameObject>>(child.ToObjectBase(), "parent") = this;
            }
        }

        for (ObjectPtr<Component>& component : components)
        {
            const Reflection& componentReflection = Reflection::GetReflection(GetTypeOf<Component>());
            if (component->GetGameObject() != this)
            {
                component = Serialization::Clone<Component>(component, false);
                componentReflection.GetFieldOf<ObjectPtr<GameObject>>(component.ToObjectBase(), "gameObject") = this;
            }
        }

        TransferFieldInfo(name);
        TransferFieldInfo(layer);
        TransferFieldInfo(localPosition);
        TransferFieldInfo(localEulerAngles);
        TransferFieldInfo(localScale);
        TransferFieldInfo(parent);
        TransferFieldInfo(children);
        TransferFieldInfo(components);
    }
    void GameObject::Awake()
    {
        if (parent != nullptr && std::ranges::find(parent->children, this) == parent->children.end())
            parent->children.emplace_back(this); //克隆时会遇到这种情况

        RenewScale();
        RenewEulerAngles();
        RenewPositionAndMatrix();

        gameObjects.emplace_back(this);

        ScriptableObject::Awake();
    }
    void GameObject::Destroy()
    {
        std::erase(gameObjects, this);

        for (const ObjectPtr<GameObject>& child : std::vector{children})
            DestroyImmediate(child);
        children.clear();
        for (const ObjectPtr<Component>& component : std::vector{components})
            DestroyImmediate(component);
        components.clear();

        if (parent != nullptr)std::erase(parent->children, this);

        ScriptableObject::Destroy();
    }
    void GameObject::Enable()
    {
        ScriptableObject::Enable();

        for (const ObjectPtr<GameObject>& child : children)
            child->UpdateActivating();
        for (const ObjectPtr<Component>& component : components)
            component->UpdateActivating();
    }
    void GameObject::Disable()
    {
        for (const ObjectPtr<GameObject>& child : children)
            child->UpdateActivating();
        for (const ObjectPtr<Component>& component : components)
            component->UpdateActivating();

        ScriptableObject::Disable();
    }
}
