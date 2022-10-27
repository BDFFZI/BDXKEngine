#include "BinaryReader.h"
#include "BDXKEngine/Base/Object/ObjectPtrBase.h"

namespace BDXKEngine
{
    BinaryReader::BinaryReader(std::iostream& buffer) : BinaryTransferrer(buffer)
    {
    }
    void BinaryReader::TransferValue(int& value)
    {
        Read(&value);
    }
    void BinaryReader::TransferValue(float& value)
    {
        Read(&value);
    }
    void BinaryReader::TransferValue(bool& value)
    {
        Read(&value);
    }
    void BinaryReader::TransferValue(Vector2& value)
    {
        Read(&value);
    }
    void BinaryReader::TransferValue(Vector3& value)
    {
        Read(&value);
    }
    void BinaryReader::TransferValue(Vector4& value)
    {
        Read(&value);
    }
    void BinaryReader::TransferValue(Color& value)
    {
        Read(&value);
    }
    void BinaryReader::TransferValue(Rect& value)
    {
        Read(&value);
    }
    void BinaryReader::TransferValue(std::string& value)
    {
        int size = 0;
        TransferValue(size);

        char* data = new char[size];
        TransferValue(data, size);

        value = data;
        value.resize(size / sizeof(char));

        delete[] data;
    }
    void BinaryReader::TransferValue(ObjectPtrBase& value)
    {
        int instanceID = 0;
        TransferValue(instanceID);
        value = Object::FindObjectOfInstanceID(instanceID);
    }
    void BinaryReader::TransferValue(Serializable& value)
    {
        value.Transfer(*this);
    }
    void BinaryReader::TransferValue(char* source, int size)
    {
        stream.read(source, size);
    }
}
