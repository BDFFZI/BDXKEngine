﻿#pragma once
#include <vector>
#include "BDXKEngine/Framework/Component/Component.h"
#include "Transform.h"
#include "BDXKEngine/Base/Data/Mathematics/Vector/Vector3.h"
#include "BDXKEngine/Base/Matrix/Matrix4x4.h"

namespace BDXKEngine
{
    class TransformManager;

    class Transform : public Component
    {
        friend TransformManager;
    public:
        ObjectPtr<Transform> GetParent();
        ObjectPtr<Transform> GetChild(int index);
        int GetChildCount() const;
        Vector3 GetPosition() const;
        Vector3 GetEulerAngles() const;
        Vector3 GetScale() const;
        Vector3 GetLocalPosition() const;
        Vector3 GetLocalEulerAngles() const;
        Vector3 GetLocalScale() const;
        Matrix4x4 GetLocalToWorldMatrix() const;
        Matrix4x4 GetWorldToLocalMatrix() const;
        Vector3 GetRight();
        Vector3 GetUp();
        Vector3 GetFront();

        void SetParent(const ObjectPtr<Transform>& parent);
        void SetLocalPosition(Vector3 value);
        void SetLocalEulerAngles(Vector3 value);
        void SetLocalScale(Vector3 value);

        std::string ToString() override;
    protected:
        void Transfer(transferer& transferer) override;
        void PreAwake() override;
        void PreDestroy() override;
    private:
        static std::vector<ObjectPtr<Transform>> rootTransforms;

        Vector3 localPosition = Vector3::zero;
        Vector3 localEulerAngles = Vector3::zero;
        Vector3 localScale = Vector3::one;
        Vector3 position = Vector3::zero;
        Vector3 eulerAngles = Vector3::zero;
        Vector3 scale = Vector3::one;
        Matrix4x4 localToWorldMatrix = Matrix4x4::identity;
        Matrix4x4 worldToLocalMatrix = Matrix4x4::identity;
        ObjectPtr<Transform> parent = nullptr;
        std::vector<ObjectPtr<Transform>> children = {};

        void RenewPositionAndMatrix(bool renewChild = true);
        void RenewEulerAngles(bool renewChild = true);
        void RenewScale(bool renewChild = true);
    };

    class TransformManager
    {
    public:
        static std::vector<ObjectPtr<Transform>> GetRootTransforms()
        {
            return Transform::rootTransforms;
        }
    };
}
