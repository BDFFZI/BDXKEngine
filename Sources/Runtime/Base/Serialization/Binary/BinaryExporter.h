#pragma once
#include "Base/Serialization/Transferrer.h"
#include "Base/Object/ObjectManager.h"

namespace BDXKEngine
{
    class BinaryExporter : public Transferrer, ObjectManager
    {
    public:
        BinaryExporter(std::iostream& stream);

        TransferDirection GetTransferDirection() override { return TransferDirection::Output; }
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
        void Write(TValue value)
        {
            stream.write(reinterpret_cast<char*>(&value), sizeof(TValue));
        }
    };
}

//std::wstringstream stream = {};
//BinaryExporter exporter = { stream };
//exporter.TransferInt(123);
//exporter.TransferVector3(Vector3{ 1,2,3 });
//exporter.TransferString(L"BDXK引擎");
//char binary[] = { 127,63,32 };
//exporter.TransferBytes(binary, 3);
//
//BinaryImporter importer = { stream };
//int intValue = importer.TransferInt();
//Vector3 vector3Value = importer.TransferVector3();
//std::wstring stringValue = importer.TransferString();
//char binaryValue[3];
//importer.TransferBytes(binaryValue, 3);
