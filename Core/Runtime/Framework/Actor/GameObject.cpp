#include "GameObject.h"
#include "Core/Runtime/Base/Object/Serialization/ObjectSerialization.h"
#include "Component.h"

namespace BDXKEngine
{
    ObjectPtr<GameObject> GameObject::Create(const std::string& name,
                                             const ObjectPtr<GameObject>& parent,
                                             const ObjectPtr<Scene>& scene,
                                             const bool instantiate)
    {
        ObjectPtr gameObject = new GameObject();
        gameObject->scene = scene;
        if (scene != nullptr)
            scene->AddObject(gameObject);
        gameObject->parent = parent;
        if (parent != nullptr)
            parent->children.emplace_back(gameObject);

        if (ObjectSerialization::IsPersistent(scene))
            ObjectSerialization::MarkPersistent(gameObject, ObjectSerialization::GetPersistent(scene));

        if (instantiate)
            AwakeObject(gameObject);

        gameObject->SetName(name);

        return gameObject;
    }
    ObjectPtr<GameObject> GameObject::FindGameObjectOfName(const std::string& name)
    {
        const auto result =
            std::ranges::find_if(allGameObjectsNotResource, [name](const GameObject* item) { return item->GetName() == name; });

        return result == allGameObjectsNotResource.end() ? nullptr : *result;
    }
    void GameObject::MarkDurable(const ObjectPtr<GameObject>& gameObject)
    {
        if (gameObject->GetParent().IsNotNull())
            throw std::exception("只有根物体允许标记持久化");

        if (gameObject->scene == Scene::GetDurableScene())
            return;
        gameObject->SetScene(Scene::GetDurableScene());
    }
    void GameObject::UnMarkDurable(const ObjectPtr<GameObject>& gameObject)
    {
        if (gameObject->scene != Scene::GetDurableScene())
            return;
        gameObject->SetScene(Scene::GetActiveScene());
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
    ObjectPtr<GameObject> GameObject::GetChild(const int index)
    {
        return children[index];
    }

    ObjectPtr<Component> GameObject::GetComponent(const Type& type) const
    {
        for (const ObjectPtr<Component>& component : components)
        {
            if (Reflection::GetReflection(component.ToObject()).CanConvertTo(type))
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

    int GameObject::GetLayer() const
    {
        return layer;
    }
    const ObjectPtr<Scene>& GameObject::GetScene() const
    {
        return scene;
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

    void GameObject::SetLayer(const int layer)
    {
        this->layer = layer;
    }
    void GameObject::SetScene(const ObjectPtr<Scene>& scene)
    {
        if (GameObject::scene != scene)
        {
            for (const ObjectPtr<GameObject>& child : children)
                child->SetScene(scene);
        }

        if (GameObject::scene != nullptr)
            GameObject::scene->RemoveObject(this);
        GameObject::scene = scene;
        if (GameObject::scene != nullptr)
            GameObject::scene->AddObject(this);
    }
    void GameObject::SetParent(const ObjectPtr<GameObject>& newparent, const bool worldPositionStays)
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

        if (parent.IsNotNull())
            std::erase(parent->children, this);
        if (newparent.IsNotNull())
        {
            newparent->children.emplace_back(this);
            SetScene(newparent->scene);
        }
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
    void GameObject::SetLocalPosition(const Vector3 value)
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
    void GameObject::SetLocalScale(const Vector3 value)
    {
        localScale = value;
        RenewScale();
        RenewPositionAndMatrix();
    }
    void GameObject::SetPosition(const Vector3 value)
    {
        if (parent.IsNull()) SetLocalPosition(value);
        else SetLocalPosition(parent->GetWorldToLocalMatrix().MultiplyPoint(value));
    }
    void GameObject::SetEulerAngles(const Vector3 value)
    {
        if (parent.IsNull()) SetLocalEulerAngles(value);
        else SetLocalEulerAngles(localEulerAngles + value - eulerAngles);
    }
    void GameObject::SetScale(const Vector3 value)
    {
        if (parent.IsNull()) SetLocalScale(value);
        else SetLocalScale(value / scale * localScale);
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
        SwitchableObject::Transfer(transferer);

        TransferFieldInfo(localPosition);
        TransferFieldInfo(localEulerAngles);
        TransferFieldInfo(localScale);
        TransferFieldInfo(layer);
        TransferFieldInfo(scene);
        TransferFieldInfo(parent);
        TransferFieldInfo(children);
        TransferFieldInfo(components);
    }
    void GameObject::Awake()
    {
        //更新变换信息
        RenewScale();
        RenewEulerAngles();
        RenewPositionAndMatrix();

        SwitchableObject::Awake();
    }
    void GameObject::Destroy()
    {
        //从父类移除 
        if (parent != nullptr)
            std::erase(parent->children, this);
        //从场景移除
        if (scene != nullptr)
            scene->RemoveObject(this);
        //删除子类
        for (const ObjectPtr<GameObject>& child : std::vector{children})
            DestroyObject(child);
        children.clear();
        //删除组件
        for (const ObjectPtr<Component>& component : std::vector{components})
            DestroyObject(component);
        components.clear();

        SwitchableObject::Destroy();
    }
    void GameObject::Enable()
    {
        SwitchableObject::Enable();

        if (IsNotResource())
        {
            assert(allGameObjectsNotResource.contains(this) == false); //事件函数执行异常
            allGameObjectsNotResource.insert(this);
        }

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

        if (IsNotResource())
        {
            assert(allGameObjectsNotResource.contains(this)); //事件函数执行异常
            allGameObjectsNotResource.erase(this);
        }

        SwitchableObject::Disable();
    }

    void GameObject::RenewPositionAndMatrix(const bool renewChild)
    {
        position = parent.IsNull() ? localPosition : parent->GetLocalToWorldMatrix().MultiplyPoint(localPosition);
        const Matrix4x4 parentLocalToWorldMatrix = parent.IsNull() ? Matrix4x4::identity : parent->GetLocalToWorldMatrix();

        localToWorldMatrix = parentLocalToWorldMatrix * Matrix4x4::TRS(localPosition, localEulerAngles, localScale);
        worldToLocalMatrix = localToWorldMatrix.GetInverse();

        if (renewChild)
            for (const ObjectPtr<GameObject>& child : children)
                child->RenewPositionAndMatrix();
    }
    void GameObject::RenewEulerAngles(const bool renewChild)
    {
        const Vector3 parentalEulerAngles = parent.IsNull() ? Vector3::zero : parent->GetEulerAngles();

        eulerAngles.x = static_cast<float>(std::fmod(parentalEulerAngles.x + localEulerAngles.x, 360));
        eulerAngles.y = static_cast<float>(std::fmod(parentalEulerAngles.y + localEulerAngles.y, 360));
        eulerAngles.z = static_cast<float>(std::fmod(parentalEulerAngles.z + localEulerAngles.z, 360));

        if (renewChild)
            for (const ObjectPtr<GameObject>& child : children)
                child->RenewEulerAngles();
    }
    void GameObject::RenewScale(const bool renewChild)
    {
        const Vector3 parentalScale = parent.IsNull() ? Vector3::one : parent->GetScale();

        scale.x = parentalScale.x * localScale.x;
        scale.y = parentalScale.y * localScale.y;
        scale.z = parentalScale.z * localScale.z;

        if (renewChild)
            for (const ObjectPtr<GameObject>& child : children)
                child->RenewScale();
    }
    void GameObject::OnAfterDeserialize()
    {
        for (ObjectPtr<GameObject>& child : children)
        {
            const Reflection& gameObjectReflection = Reflection::GetReflection(GetTypeOf<GameObject>());
            if (child->GetParent() != this)
            {
                if (child->GetParent().IsNull()) //子类是预制体
                {
                    //矫正子类
                    gameObjectReflection.GetFieldOf<ObjectPtr<GameObject>>(child.ToObject(), "parent") = this;
                }
                else //自身是克隆出来的物体
                {
                    //克隆子类
                    child = ObjectSerialization::Clone<GameObject>(child, false);
                    gameObjectReflection.GetFieldOf<ObjectPtr<GameObject>>(child.ToObject(), "parent") = this;
                }
            }
        }

        for (ObjectPtr<Component>& component : components)
        {
            const Reflection& componentReflection = Reflection::GetReflection<Component>();
            if (component->GetGameObject() != this) //自身是克隆出来的物体
            {
                //克隆组件
                component = ObjectSerialization::Clone<Component>(component, false);
                componentReflection.GetFieldOf<ObjectPtr<GameObject>>(component.ToObject(), "gameObject") = this;
            }
        }
    }
}
