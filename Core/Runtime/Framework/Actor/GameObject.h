#pragma once
#include "SwitchableObject.h"
#include "Core/Runtime/Base/Data/Math/Matrix/Matrix4x4.h"
#include "Core/Runtime/Base/Data/Math/Vector/Vector3.h"
#include "Core/Runtime/Base/Object/Serialization/ObjectSerialization.h"
#include "Core/Runtime/Framework/Scene/Scene.h"

namespace BDXKEngine
{
    class Component;

    class GameObject : public SwitchableObject, public DeserializeEvent
    {
    public:
        static ObjectPtr<GameObject> Create(const std::string& name = "New GameObject",
                                            const ObjectPtr<GameObject>& parent = nullptr,
                                            const ObjectPtr<Scene>& scene = Scene::GetActiveScene(),
                                            bool instantiate = true);
        static ObjectPtr<GameObject> FindGameObjectOfName(const std::string& name);
        static void MarkDurable(const ObjectPtr<GameObject>& gameObject);
        static void UnMarkDurable(const ObjectPtr<GameObject>& gameObject);

        bool GetIsActivating() const override;
        ObjectPtr<Component> GetComponent(const Type& type) const;
        template <typename TComponent, class TBack = TComponent>
        ObjectPtr<TBack> GetComponent() const
        {
            for (const ObjectPtr<Component>& component : components)
            {
                auto* target = component.ToObject<TComponent>();
                if (target != nullptr)
                    return dynamic_cast<TBack*>(target);
            }
            return nullptr;
        }
        template <typename TComponent, class TBack = TComponent>
        bool TryGetComponent(ObjectPtr<TBack>& component)
        {
            component = GetComponent<TComponent, TBack>();
            return component.IsNotNull();
        }
        std::vector<ObjectPtr<Component>> GetComponents() const;
        template <typename TComponent, class TBack = TComponent>
        std::vector<ObjectPtr<TBack>> GetComponents() const
        {
            std::vector<ObjectPtr<TBack>> back = {};
            for (const ObjectPtr<Component>& component : components)
            {
                TComponent* target = component.ToObject<TComponent>();
                if (target != nullptr)back.push_back(dynamic_cast<TBack*>(target));
            }
            return back;
        }
        template <typename TComponent, class TBack = TComponent>
        ObjectPtr<TBack> GetComponentInParent() const
        {
            ObjectPtr node = this;
            while (node.IsNotNull())
            {
                auto target = node->GetComponent<TComponent, TBack>();
                if (target.IsNotNull())return target;

                node = node->GetParent();
            }

            return nullptr;
        }
        int GetChildCount() const;
        ObjectPtr<GameObject> GetChild(int index);
        const ObjectPtr<Scene>& GetScene() const;
        ObjectPtr<GameObject> GetParent();
        int GetLayer() const;
        Vector3 GetLocalPosition() const;
        Vector3 GetLocalEulerAngles() const;
        Vector3 GetLocalScale() const;
        Matrix4x4 GetLocalToWorldMatrix() const;
        Matrix4x4 GetWorldToLocalMatrix() const;
        Vector3 GetPosition() const;
        Vector3 GetEulerAngles() const;
        Vector3 GetScale() const;
        Vector3 GetRight() const;
        Vector3 GetUp() const;
        Vector3 GetFront() const;

        void SetScene(const ObjectPtr<Scene>& scene);
        void SetParent(const ObjectPtr<GameObject>& parent, bool worldPositionStays = true);
        void SetLayer(int layer);
        void SetLocalPosition(Vector3 value);
        void SetLocalEulerAngles(Vector3 value);
        void SetLocalScale(Vector3 value);
        void SetPosition(Vector3 value);
        void SetEulerAngles(Vector3 value);
        void SetScale(Vector3 value);
        void AddComponent(const ObjectPtr<Component>& component);
        void RemoveComponent(const ObjectPtr<Component>& component);

        void Transfer(Transferer& transferer) override;

    protected:
        void Awake() override;
        void Destroy() override;

        void Enable() override;
        void Disable() override;

    private:
        inline static std::unordered_set<GameObject*> allGameObjectsNotResource = {};

        ObjectPtr<Scene> scene = nullptr;
        ObjectPtr<GameObject> parent = nullptr;
        std::vector<ObjectPtr<GameObject>> children = {};
        std::vector<ObjectPtr<Component>> components;
        int layer = 0; //第0-31层
        Vector3 localPosition = Vector3::zero;
        Vector3 localEulerAngles = Vector3::zero;
        Vector3 localScale = Vector3::one;

        Vector3 position = Vector3::zero;
        Vector3 eulerAngles = Vector3::zero;
        Vector3 scale = Vector3::one;
        Matrix4x4 localToWorldMatrix = Matrix4x4::identity;
        Matrix4x4 worldToLocalMatrix = Matrix4x4::identity;

        void RenewPositionAndMatrix(bool renewChild = true);
        void RenewEulerAngles(bool renewChild = true);
        void RenewScale(bool renewChild = true);
        void OnAfterDeserialize() override;
    };
}

#include "Component.h"
