#pragma once
#include "BDXKEngine/Base/Object/ObjectManager.h"
#include "BinaryTransferrer.h"

namespace BDXKEngine
{
    class BinaryWriter : public BinaryTransferrer, ObjectManager
    {
    public:
        BinaryWriter(std::iostream& buffer);

        TransferDirection GetTransferDirection() override { return TransferDirection::Output; }
        void TransferValue(int& value) override;
        void TransferValue(float& value) override;
        void TransferValue(bool& value) override;
        void TransferValue(Vector2& value) override;
        void TransferValue(Vector3& value) override;
        void TransferValue(Vector4& value) override;
        void TransferValue(Color& value) override;
        void TransferValue(Rect& value) override;
        void TransferValue(std::string& value) override;
        void TransferValue(ObjectPtrBase& value) override;
        void TransferValue(ISerializable& value) override;
        void TransferValue(std::vector<ObjectPtrBase>& vector) override;
        void TransferValue(char* source, int size) override;
        //void TransferVector( std::vector<ISerializable>& vector) override;
    private:
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
