#include "BinaryReader.h"

namespace BDXKEngine
{
    BinaryReader::BinaryReader(const std::shared_ptr<std::iostream>& stream) : BinaryTransferer(stream, true)
    {
    }

    void BinaryReader::TransferString(std::string& value)
    {
        int size = 0;
        ReadStreamTo(size);
        char* data = new char[size];
        ReadStreamTo(data, size);

        value = data;
        value.resize(size);

        delete[] data;
    }

    // void BinaryReader::TransferValue(int& value)
    // {
    //     
    // }
    // void BinaryReader::TransferValue(float& value)
    // {
    //     
    // }
    // void BinaryReader::TransferValue(bool& value)
    // {
    //    
    // }
    // void BinaryReader::TransferValue(Vector2& value)
    // {
    //     Read(&value);
    // }
    // void BinaryReader::TransferValue(Vector3& value)
    // {
    //     Read(&value);
    // }
    // void BinaryReader::TransferValue(Vector4& value)
    // {
    //     Read(&value);
    // }
    // void BinaryReader::TransferValue(Color& value)
    // {
    //     Read(&value);
    // }
    // void BinaryReader::TransferValue(Rect& value)
    // {
    //     Read(&value);
    // }
    // void BinaryReader::TransferValue(std::string& value)
    // {

    // }
    // void BinaryReader::TransferValue(ObjectPtrBase& value)
    // {
    //     int instanceID = 0;
    //     TransferValue(instanceID);
    //     value = Object::FindObjectOfInstanceID(instanceID);
    // }
    // void BinaryReader::TransferValue(Serialization& value)
    // {
    //     value.Transfer(*this);
    // }
    // void BinaryReader::TransferValue(char* source, int size)
    // {
    //     stream.read(source, size);
    // }
}
