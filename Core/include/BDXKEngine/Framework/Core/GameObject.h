#pragma once
#include "ScriptableObject.h"
#include "BDXKEngine/Base/Data/Mathematics/Matrix/Matrix4x4.h"
#include "BDXKEngine/Base/Data/Mathematics/Vector/Vector3.h"
#include "BDXKEngine/Base/Reflection/Reflection.h"

namespace BDXKEngine
{
    class Component;

    class GameObject : public ScriptableObject
    {
    public:
        static const std::vector<ObjectPtr<GameObject>>& GetGameObjects();
        static const std::vector<ObjectPtr<GameObject>>& GetGameObjectsHiding();
        
        static ObjectPtr<GameObject> Create(const std::string& name = "New GameObject", const ObjectPtr<GameObject>& parent = nullptr);
        static ObjectPtr<GameObject> Find(const std::string& name);
        /**
         * \brief 近似的DontDestroyOnLoad
         * 被Hide的物体将不被系统管理，Find无法查询到该物体，同样跳转场景时也不会被删除
         * \param gameObject 被隐藏的物体
         */
        static void Hide(const ObjectPtr<GameObject>& gameObject);
        static void UnHide(const ObjectPtr<GameObject>& gameObject);
        static void Clear();

        bool GetIsActivating() const override;

        ObjectPtr<GameObject> GetParent();
        int GetChildCount() const;
        ObjectPtr<GameObject> GetChild(int index);
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

        const std::string& GetName() const;
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

        void SetName(const std::string& name);
        void SetLayer(int layer);
        void SetParent(const ObjectPtr<GameObject>& parent, bool worldPositionStays = true);
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
        static std::vector<ObjectPtr<GameObject>> gameObjects;
        static std::vector<ObjectPtr<GameObject>> gameObjectsHiding;

        std::string name;
        int layer = 0; //第0-31层
        Vector3 position = Vector3::zero;
        Vector3 eulerAngles = Vector3::zero;
        Vector3 scale = Vector3::one;
        Matrix4x4 localToWorldMatrix = Matrix4x4::identity;
        Matrix4x4 worldToLocalMatrix = Matrix4x4::identity;

        Vector3 localPosition = Vector3::zero;
        Vector3 localEulerAngles = Vector3::zero;
        Vector3 localScale = Vector3::one;
        ObjectPtr<GameObject> parent = nullptr;
        std::vector<ObjectPtr<GameObject>> children = {};
        std::vector<ObjectPtr<Component>> components;

        void RenewPositionAndMatrix(bool renewChild = true);
        void RenewEulerAngles(bool renewChild = true);
        void RenewScale(bool renewChild = true);
    };
}

#include "Component.h"
