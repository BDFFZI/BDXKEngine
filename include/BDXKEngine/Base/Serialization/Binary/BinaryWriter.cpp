#include "BinaryWriter.h"
#include "BDXKEngine/Base/Object/ObjectPtrBase.h"

namespace BDXKEngine
{
    BinaryWriter::BinaryWriter(std::iostream& buffer) : BinaryTransferrer(buffer)
    {
    }

    void BinaryWriter::TransferValue(int& value)
    {
        Write(value);
    }
    void BinaryWriter::TransferValue(float& value)
    {
        Write(value);
    }
    void BinaryWriter::TransferValue(bool& value)
    {
        Write(value);
    }
    void BinaryWriter::TransferValue(Vector2& value)
    {
        Write(value);
    }
    void BinaryWriter::TransferValue(Vector3& value)
    {
        Write(value);
    }
    void BinaryWriter::TransferValue(Vector4& value)
    {
        Write(value);
    }
    void BinaryWriter::TransferValue(Color& value)
    {
        Write(value);
    }
    void BinaryWriter::TransferValue(Rect& value)
    {
        Write(value);
    }
    void BinaryWriter::TransferValue(std::string& value)
    {
        int size = static_cast<int>(value.size() * sizeof(char));
        char* data = value.data();

        TransferValue(size);
        TransferValue(data, size);
    }
    void BinaryWriter::TransferValue(ObjectPtrBase& value)
    {
        int instanceID = value.GetInstanceID();
        TransferValue(instanceID);
    }
    void BinaryWriter::TransferValue(Serializable& value)
    {
        value.Transfer(*this);
    }
    void BinaryWriter::TransferValue(char* source, int size)
    {
        stream.write(source, size);
    }
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

