#pragma once
#include "BDXKEngine/Base/Serialization/Transferrer.h"
#include "BDXKEngine/Base/Object/ObjectManager.h"

namespace BDXKEngine
{
    class BinaryExporter : public Transferrer, ObjectManager
    {
    public:
        BinaryExporter(std::iostream& stream);

        TransferDirection GetTransferDirection() override { return TransferDirection::Output; }
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
        void Write(TValue value)
        {
            stream.write(reinterpret_cast<char*>(&value), sizeof(TValue));
        }
    };
}

//std::stringstream stream = {};
//BinaryExporter exporter = { stream };
//exporter.TransferInt(123);
//exporter.TransferVector3(Vector3{ 1,2,3 });
//exporter.TransferString("BDXK引擎");
//char binary[] = { 127,63,32 };
//exporter.TransferBytes(binary, 3);
//
//BinaryImporter importer = { stream };
//int intValue = importer.TransferInt();
//Vector3 vector3Value = importer.TransferVector3();
//std::string stringValue = importer.TransferString();
//char binaryValue[3];
//importer.TransferBytes(binaryValue, 3);
