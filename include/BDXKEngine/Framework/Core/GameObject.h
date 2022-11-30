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
        static ObjectPtr<GameObject> Create(const std::string& name = "New GameObject",const ObjectPtr<GameObject>& parent = nullptr);
        static const std::vector<ObjectPtr<GameObject>>& GetGameObjects();
        static  ObjectPtr<GameObject> Find(const std::string& name);

        bool GetIsActivating() const override;

        ObjectPtr<GameObject> GetParent();
        int GetChildCount() const;
        ObjectPtr<GameObject> GetChild(int index);
        ObjectPtr<Component> GetComponent(const Type& type) const;
        template <typename TComponent>
        ObjectPtr<TComponent> GetComponent() const
        {
            for (const ObjectPtr<Component>& component : components)
            {
                ObjectPtr<TComponent> target = component.ToObjectPtr<TComponent>();
                if (target.IsNull() == false)
                    return target;
            }
            return nullptr;
        }
        std::vector<ObjectPtr<Component>> GetComponents() const;

        Vector3 GetPosition() const;
        Vector3 GetEulerAngles() const;
        Vector3 GetScale() const;
        Vector3 GetLocalPosition() const;
        Vector3 GetLocalEulerAngles() const;
        Vector3 GetLocalScale() const;
        Matrix4x4 GetLocalToWorldMatrix() const;
        Matrix4x4 GetWorldToLocalMatrix() const;
        Vector3 GetRight() const;
        Vector3 GetUp() const;
        Vector3 GetFront() const;
        
        void SetParent(const ObjectPtr<GameObject>& parent);
        void SetLocalPosition(Vector3 value);
        void SetLocalEulerAngles(Vector3 value);
        void SetLocalScale(Vector3 value);
        
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
