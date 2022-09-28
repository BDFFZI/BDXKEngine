#pragma once
#include "Base/Object/ObjectManager.h"
#include "Base/Serialization/Transferrer.h"

namespace BDXKEngine
{
    class BinaryImporter : public Transferrer, ObjectManager
    {
    public:
        BinaryImporter(std::iostream& stream);

        TransferDirection GetTransferDirection() override { return TransferDirection::Input; }
        void TransferInt(std::wstring key, int& value) override;
        void TransferFloat(std::wstring key, float& value) override;
        void TransferBool(std::wstring key, bool& value) override;
        void TransferVector2(std::wstring key, Vector2& value) override;
        void TransferVector3(std::wstring key, Vector3& value) override;
        void TransferVector4(std::wstring key, Vector4& value) override;
        void TransferColor(std::wstring key, Color& value) override;
        void TransferRect(std::wstring key, Rect& value) override;
        void TransferString(std::wstring key, std::wstring& value) override;
        void TransferObjectPtr(std::wstring key, ObjectPtrBase& value) override;
        void TransferBytes(std::wstring key, void* source, int size) override;
    private:
        std::iostream& stream;

        template <typename TValue>
        void Read(TValue* value)
        {
            stream.read(reinterpret_cast<char*>(value), sizeof(TValue));
        }
    };
}
