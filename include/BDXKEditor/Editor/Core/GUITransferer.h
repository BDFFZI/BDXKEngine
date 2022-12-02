#pragma once
#include "BDXKEngine/Base/Data/Graphics/Color.h"
#include "BDXKEngine/Base/Data/Graphics/Rect.h"
#include "BDXKEngine/Base/Data/Mathematics/Matrix/Matrix4x4.h"
#include "BDXKEngine/Base/Data/Mathematics/Vector/Vector4.h"
#include "BDXKEngine/Base/Object/ObjectPtrBase.h"
#include "BDXKEngine/Base/Serialization/Core/IOTransferer.h"

namespace BDXKEditor
{
    using namespace BDXKEngine;

    class GUITransferer : public Transferer
    {
    public:
        std::string GetFieldPath() const { return fieldPath; }
        std::string GetFieldName() const { return fieldName; }
        std::string GetFieldID() const { return GetFieldName() + "##" + GetFieldPath(); }
        std::string GetFieldID(const std::string& name) const { return name + "##" + GetFieldPath() + name; }
        void SetClickObjectEvent(const std::function<void(const ObjectPtrBase&)>& clickObjectEvent);

        void PushPath(const std::string& key) override;
        void PopPath(std::string& key) override;

        void TransferInt(int& value) const;
        void TransferFloat(float& value) const;
        void TransferBool(bool& value) const;
        void TransferVector2(Vector2& value) const;
        void TransferVector3(Vector3& value) const;
        void TransferVector4(Vector4& value) const;
        void TransferMatrix4x4(Matrix4x4& value) const;
        void TransferColor(Color& value) const;
        void TransferRect(Rect& value) const;
        void TransferString(std::string& value) const;
        void TransferBytes(const std::vector<char>& value) const;
        void TransferObjectPtrBase(ObjectPtrBase& value) const;
    private:
        std::string fieldPath;
        std::string fieldName;
        bool isInPath = false;
        size_t isNested = -1;
        std::function<void(const ObjectPtrBase&)> clickObjectEvent;

        CustomTransferFunc(int, TransferInt)
        CustomTransferFunc(float, TransferFloat)
        CustomTransferFunc(bool, TransferBool)
        CustomTransferFunc(Vector2, TransferVector2)
        CustomTransferFunc(Vector3, TransferVector3)
        CustomTransferFunc(Vector4, TransferVector4)
        CustomTransferFunc(Matrix4x4, TransferMatrix4x4)
        CustomTransferFunc(Color, TransferColor)
        CustomTransferFunc(Rect, TransferRect)
        CustomTransferFunc(std::string, TransferString)
        CustomTransferFunc(std::vector<char>, TransferBytes)
        CustomTransferFunc(ObjectPtrBase, TransferObjectPtrBase)
    };
}
