#pragma once
#include "Jsontransferer.h"
#include <crossguid/guid.hpp>
#include "Core/Runtime/Base/Data/Graphics/Color.h"
#include "Core/Runtime/Base/Data/Graphics/Rect.h"
#include "Core/Runtime/Base/Data/Math/Matrix/Matrix4x4.h"

namespace BDXKEngine
{
    class JsonReader : public JsonTransferer
    {
    public:
        void TransferJson(std::string key, std::string& value) override;

        bool IsImporter() override;
        void Reset(std::string& data) override;

        void PushPath(const std::string& key) override;
        void PopPath(std::string& key) override;

        void TransferInt(int& value);
        void TransferFloat(float& value);
        void TransferBool(bool& value);
        void TransferString(std::string& value);
        void TransferBytes(std::vector<char>& value);
        void TransferGuid(xg::Guid& guid);
        void TransferVector2(Vector2& value);
        void TransferVector3(Vector3& value);
        void TransferVector4(Vector4& value);
        void TransferMatrix4x4(Matrix4x4& value);
        void TransferColor(Color& value);
        void TransferRect(Rect& value);

        JsonReader();

    protected:
        std::function<void(void*, const Type&)> GetTransferFunc(const Type& type) override;

    private:
        inline static Type nullType = GetTypeOf<char>();
    };
}
