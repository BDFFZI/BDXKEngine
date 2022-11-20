#pragma once
#include "Jsontransferer.h"
#include "BDXKEngine/Base/Data/Mathematics/Vector/Vector4.h"
#include "BDXKEngine/Base/Data/Graphics/Color.h"
#include "BDXKEngine/Base/Data/Graphics/Rect.h"

namespace BDXKEngine
{
    class JsonExporter : public JsonTransferer
    {
    public:
        void TransferJson(std::string key, std::string& value) override;

        void Reset(std::string& data) override;
        void PushPath(const std::string& key) override;
        void PopPath(std::string& key) override;
    protected:
        void TransferInt(const int& value);
        void TransferFloat(const float& value);
        void TransferBool(const bool& value);
        void TransferVector2(const Vector2& value);
        void TransferVector3(const Vector3& value);
        void TransferVector4(const Vector4& value);
        void TransferColor(const Color& value);
        void TransferRect(const Rect& value);
        void TransferString(const std::string& value);
        void TransferBytes(const std::vector<char>& value);
    private:
        CustomTransferFunc(int, TransferInt)
        CustomTransferFunc(float, TransferFloat)
        CustomTransferFunc(bool, TransferBool)
        CustomTransferFunc(Vector2, TransferVector2)
        CustomTransferFunc(Vector3, TransferVector3)
        CustomTransferFunc(Vector4, TransferVector4)
        CustomTransferFunc(Color, TransferColor)
        CustomTransferFunc(Rect, TransferRect)
        CustomTransferFunc(std::string, TransferString)
        CustomTransferFunc(std::vector<char>, TransferBytes)
    };
}
