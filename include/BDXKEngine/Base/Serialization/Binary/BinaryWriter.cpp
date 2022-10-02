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
    void BinaryWriter::TransferValue(ISerializable& value)
    {
        value.Transfer(*this);
    }
    void BinaryWriter::TransferValue(std::vector<ObjectPtrBase>& vector)
    {
        int count = static_cast<int>(vector.size());
        TransferValue(count);
        for (int i = 0; i < count; i++)
            TransferValue(vector[i]);
    }
    void BinaryWriter::TransferValue(char* source, int size)
    {
        stream.write(source, size);
    }
    // void BinaryExporter::TransferVector( std::vector<ISerializable>& vector)
    // {
    //     int count = static_cast<int>(vector.size());
    //     TransferInt(key + "_count", count);
    //     for (int i = 0; i < count; i++)
    //         vector[i].Transfer(*this);
    // }
}
