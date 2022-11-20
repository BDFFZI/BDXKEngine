#pragma once
#include "Jsontransferer.h"
#include "BDXKEngine/Base/Data/Mathematics/Vector/Vector4.h"
#include "BDXKEngine/Base/Data/Graphics/Color.h"
#include "BDXKEngine/Base/Data/Graphics/Rect.h"

namespace BDXKEngine
{
    class JsonImporter : public JsonTransferer
    {
    public:
        void TransferJson(std::string key, std::string& value) override;

        void Reset(std::string& data) override;
        void PushPath(const std::string& key) override;
        void PopPath(std::string& key) override;
    protected:
        void TransferInt(int& value);
        void TransferFloat(float& value);
        void TransferBool(bool& value);
        void TransferVector2(Vector2& value);
        void TransferVector3(Vector3& value);
        void TransferVector4(Vector4& value);
        void TransferColor(Color& value);
        void TransferRect(Rect& value);
        void TransferString(std::string& value);
        void TransferBytes(std::vector<char>& value);
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
