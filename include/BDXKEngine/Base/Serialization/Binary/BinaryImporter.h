#pragma once
#include "BDXKEngine/Base/Object/ObjectManager.h"
#include "BDXKEngine/Base/Serialization/Transferrer.h"

namespace BDXKEngine
{
    class BinaryImporter : public Transferrer, ObjectManager
    {
    public:
        BinaryImporter(std::iostream& stream);

        TransferDirection GetTransferDirection() override { return TransferDirection::Input; }
        void TransferInt(std::string key, int& value) override;
        void TransferFloat(std::string key, float& value) override;
        void TransferBool(std::string key, bool& value) override;
        void TransferVector2(std::string key, Vector2& value) override;
        void TransferVector3(std::string key, Vector3& value) override;
        void TransferVector4(std::string key, Vector4& value) override;
        void TransferColor(std::string key, Color& value) override;
        void TransferRect(std::string key, Rect& value) override;
        void TransferString(std::string key, std::string& value) override;
        void TransferObjectPtr(std::string key, ObjectPtrBase& value) override;
        void TransferBytes(std::string key, void* source, int size) override;
    private:
        std::iostream& stream;

        template <typename TValue>
        void Read(TValue* value)
        {
            stream.read(reinterpret_cast<char*>(value), sizeof(TValue));
        }
    };
}
