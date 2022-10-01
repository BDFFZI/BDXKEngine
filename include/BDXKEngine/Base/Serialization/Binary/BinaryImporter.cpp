#include "BinaryImporter.h"
#include "BDXKEngine/Base/Object/ObjectPtrBase.h"

namespace BDXKEngine
{
    BinaryImporter::BinaryImporter(std::iostream& stream) : stream(stream)
    {
    }
    void BinaryImporter::TransferInt(std::string key, int& value)
    {
        Read(&value);
    }
    void BinaryImporter::TransferFloat(std::string key, float& value)
    {
        Read(&value);
    }
    void BinaryImporter::TransferBool(std::string key, bool& value)
    {
        Read(&value);
    }
    void BinaryImporter::TransferVector2(std::string key, Vector2& value)
    {
        Read(&value);
    }
    void BinaryImporter::TransferVector3(std::string key, Vector3& value)
    {
        Read(&value);
    }
    void BinaryImporter::TransferVector4(std::string key, Vector4& value)
    {
        Read(&value);
    }
    void BinaryImporter::TransferColor(std::string key, Color& value)
    {
        Read(&value);
    }
    void BinaryImporter::TransferRect(std::string key, Rect& value)
    {
        Read(&value);
    }
    void BinaryImporter::TransferString(std::string key, std::string& value)
    {
        int size = 0;
        TransferInt({}, size);

        char* data = new char[size];
        TransferBytes({}, data, size);

        value = data;
        value.resize(size / sizeof(char));

        delete[] data;
    }
    void BinaryImporter::TransferObjectPtr(std::string key, ObjectPtrBase& value)
    {
        int instanceID = 0;
        TransferInt({}, instanceID);
        value = Object::FindObjectOfInstanceID(instanceID);
    }
    void BinaryImporter::TransferBytes(std::string key, void* source, int size)
    {
        stream.read(static_cast<char*>(source), size);
    }
}
