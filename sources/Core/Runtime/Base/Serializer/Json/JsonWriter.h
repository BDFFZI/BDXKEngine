#pragma once
#include "Jsontransferer.h"
#include <crossguid/guid.hpp>
#include "Core/Runtime/Base/Data/Graphics/Color.h"
#include "Core/Runtime/Base/Data/Graphics/Rect.h"
#include "Core/Runtime/Base/Data/Math/Matrix/Matrix4x4.h"

namespace BDXKEngine
{
    class JsonWriter : public JsonTransferer
    {
    public:
        void TransferJson(std::string key, std::string& value) override;

        bool IsImporter() override;
        void Reset(std::string& data) override;
        
        void PushPath(const std::string& key) override;
        void PopPath(std::string& key) override;

        void TransferInt(const int& value);
        void TransferFloat(const float& value);
        void TransferBool(const bool& value);
        void TransferString(const std::string& value);
        void TransferBytes(const std::vector<char>& value);
        void TransferGuid(const xg::Guid& guid);
        void TransferVector2(const Vector2& value);
        void TransferVector3(const Vector3& value);
        void TransferVector4(const Vector4& value);
        void TransferMatrix4x4(const Matrix4x4& value);
        void TransferColor(const Color& value);
        void TransferRect(const Rect& value);

        JsonWriter();
    };
}
