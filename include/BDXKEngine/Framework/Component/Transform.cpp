﻿#include "Transform.h"
#include<cmath>
#include<exception>
#include "BDXKEngine/Base/Extension/Vector.h"
#include "BDXKEngine/Framework/GameObject.h"

namespace BDXKEngine
{
    std::vector<ObjectPtr<Transform>> Transform::rootTransforms{};

    ObjectPtr<Transform> Transform::GetParent()
    {
        return parent;
    }
    ObjectPtr<Transform> Transform::GetChild(int index)
    {
        return children[index];
    }
    int Transform::GetChildCount() const
    {
        return static_cast<int>(children.size());
    }
    Vector3 Transform::GetPosition() const
    {
        return position;
    }
    Vector3 Transform::GetEulerAngles() const
    {
        return eulerAngles;
    }
    Vector3 Transform::GetScale() const
    {
        return scale;
    }
    Vector3 Transform::GetLocalPosition() const
    {
        return localPosition;
    }
    Vector3 Transform::GetLocalEulerAngles() const
    {
        return localEulerAngles;
    }
    Vector3 Transform::GetLocalScale() const
    {
        return localScale;
    }
    Matrix4x4 Transform::GetLocalToWorldMatrix() const
    {
        return localToWorldMatrix;
    }
    Matrix4x4 Transform::GetWorldToLocalMatrix() const
    {
        return worldToLocalMatrix;
    }
    Vector3 Transform::GetRight()
    {
        return localToWorldMatrix.MultiplyVector(Vector3::right);
    }
    Vector3 Transform::GetUp()
    {
        return localToWorldMatrix.MultiplyVector(Vector3::up);
    }
    Vector3 Transform::GetFront()
    {
        return localToWorldMatrix.MultiplyVector(Vector3::front);
    }

    void Transform::SetParent(ObjectPtr<Transform> newparent)
    {
        //嵌套检查
        if (newparent.IsNull() == false)
        {
            const ObjectPtr<Transform> current = this;
            ObjectPtr<Transform> newUpLayer = newparent;
            do
            {
                if (newUpLayer == current)
                    throw std::exception("你在试图让一个父物体或自身成为其的孩子，这会导致嵌套循环，是不允许的。");
                newUpLayer = newUpLayer->parent;
            }
            while (newUpLayer.IsNull() == false);
        }

        const bool oldActivating = GetGameObject()->GetIsActivating();

        //解绑旧父物体
        if (parent.IsNull() == false)
            parent->children.erase(
                std::find(parent->children.begin(), parent->children.end(), this));
        else rootTransforms.erase(std::find(rootTransforms.begin(), rootTransforms.end(), this));
        //设置新父物体
        this->parent = std::move(newparent);
        //绑定新父物体
        if (parent.IsNull() == false)parent->children.emplace_back(this);
        else rootTransforms.emplace_back(this);

        RenewScale();
        RenewEulerAngles();
        RenewPositionAndMatrix();

        if (oldActivating != GetGameObject()->GetIsActivating())
            GetGameObject()->UpdateActivating();
    }
    void Transform::SetLocalPosition(Vector3 value)
    {
        localPosition = value;
        RenewPositionAndMatrix();
    }
    void Transform::SetLocalEulerAngles(Vector3 value)
    {
        value.x = static_cast<float>(std::fmod(value.x, 360));
        value.y = static_cast<float>(std::fmod(value.y, 360));
        value.z = static_cast<float>(std::fmod(value.z, 360));
        localEulerAngles = value;
        RenewEulerAngles();
        RenewPositionAndMatrix();
    }
    void Transform::SetLocalScale(Vector3 value)
    {
        localScale = value;
        RenewScale();
        RenewPositionAndMatrix();
    }

    void Transform::RenewPositionAndMatrix(bool renewChild)
    {
        position = parent.IsNull() ? localPosition : parent->GetLocalToWorldMatrix().MultiplyPoint(localPosition);
        Matrix4x4 parentLocalToWorldMatrix = parent.IsNull() ? Matrix4x4::identity : parent->GetLocalToWorldMatrix();

        localToWorldMatrix = parentLocalToWorldMatrix * Matrix4x4::TRS(localPosition, localEulerAngles, localScale);
        worldToLocalMatrix = localToWorldMatrix.GetInverse();

        if (renewChild)
            for (const ObjectPtr<Transform>& child : children)
                child->RenewPositionAndMatrix();
    }
    void Transform::RenewEulerAngles(bool renewChild)
    {
        const Vector3 parentalEulerAngles = parent.IsNull() ? Vector3::zero : parent->GetEulerAngles();

        eulerAngles.x = static_cast<float>(std::fmod(parentalEulerAngles.x + localEulerAngles.x, 360));
        eulerAngles.y = static_cast<float>(std::fmod(parentalEulerAngles.y + localEulerAngles.y, 360));
        eulerAngles.z = static_cast<float>(std::fmod(parentalEulerAngles.z + localEulerAngles.z, 360));

        if (renewChild)
            for (const ObjectPtr<Transform>& child : children)
                child->RenewEulerAngles();
    }
    void Transform::RenewScale(bool renewChild)
    {
        const Vector3 parentalScale = parent.IsNull() ? Vector3::one : parent->GetScale();

        scale.x = parentalScale.x * localScale.x;
        scale.y = parentalScale.y * localScale.y;
        scale.z = parentalScale.z * localScale.z;

        if (renewChild)
            for (const ObjectPtr<Transform>& child : children)
                child->RenewScale();
    }

    void Transform::Transfer(Transferrer& transferrer)
    {
        Component::Transfer(transferrer);

        transferrer.TransferVector3(nameof(localPosition), localPosition);
        transferrer.TransferVector3(nameof(localEulerAngles), localEulerAngles);
        transferrer.TransferVector3(nameof(localScale), localScale);

        int childrenCount = static_cast<int>(children.size());
        transferrer.TransferInt(nameof(childrenCount), childrenCount);

        if (transferrer.GetTransferDirection() == TransferDirection::Input)children.resize(childrenCount);
        for (int i = 0; i < childrenCount; i++)
        {
            ObjectPtr<Transform>& child = children[i];
            transferrer.TransferObjectPtr("children_" + std::to_string(i), child);

            if (transferrer.GetTransferDirection() == TransferDirection::Input)
            {
                const ObjectPtr childGameObject = InstantiateNoAwake(child->GetGameObject().ToObject());
                child = childGameObject->GetTransform();
                child->parent = this;
            }
        }
    }

    void Transform::Awake()
    {
        Component::Awake();

        if (parent == nullptr)
            rootTransforms.emplace_back(this);

        RenewScale(false);
        RenewEulerAngles(false);
        RenewPositionAndMatrix(false);

        for (const ObjectPtr<Transform>& child : children)
            Object::Awake(child->GetGameObject().ToObjectBase());
    }
    void Transform::Destroy()
    {
        for (const ObjectPtr<Transform>& child : children)
            Object::Destroy(child->GetGameObject().ToObjectBase());
        children.clear();

        if (parent.IsNull() && GetIsRunning())
            Vector::Remove(rootTransforms, {this});

        Component::Destroy();
    }

    std::string Transform::ToString()
    {
        std::stringstream stream;
        stream << Component::ToString();
        stream << "位置：" << localPosition.ToString() << std::endl;
        stream << "旋转：" << localEulerAngles.ToString() << std::endl;
        stream << "缩放：" << localScale.ToString() << std::endl;
        stream << "父亲：" << (parent.IsNull() == false ? parent->GetGameObject()->GetName() : "nullptr") << std::endl;
        stream << "孩子数量：" << std::to_string(GetChildCount()) << std::endl;
        return stream.str();
    }
}