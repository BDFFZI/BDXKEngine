#pragma once
#include "Core/Runtime/Base/Data/Graphics/Color.h"
#include "Core/Runtime/Base/Data/Graphics/Rect.h"
#include "Core/Runtime/Base/Data/Math/Matrix/Matrix4x4.h"
#include "Core/Runtime/Base/Data/Math/Vector/Vector4.h"
#include "Core/Runtime/Base/Object/Object.h"
#include "Core/Runtime/Base/Serializer/IOTransferer.h"
#include <crossguid/guid.hpp>

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
        void TransferString(std::string& value) const;
        void TransferBytes(const std::vector<char>& value) const;
        void TransferGuid(xg::Guid& guid) const;
        void TransferVector2(Vector2& value) const;
        void TransferVector3(Vector3& value) const;
        void TransferVector4(Vector4& value) const;
        void TransferMatrix4x4(Matrix4x4& value) const;
        void TransferColor(Color& value) const;
        void TransferRect(Rect& value) const;
        void TransferObjectPtr(ObjectPtrBase& value) const;

        GUITransferer();

        std::string fieldPath;
        std::string fieldName;
        bool isInPath = false;
        int isNested = -1;
        std::function<void(const ObjectPtrBase&)> clickObjectEvent;
    };
}
