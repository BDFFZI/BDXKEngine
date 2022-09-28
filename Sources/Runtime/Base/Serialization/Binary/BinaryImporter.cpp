#include "BinaryImporter.h"
#include "Base/Object/ObjectPtrBase.h"

namespace BDXKEngine
{
    BinaryImporter::BinaryImporter(std::iostream& stream) : stream(stream)
    {
    }
    void BinaryImporter::TransferInt(std::wstring key, int& value)
    {
        Read(&value);
    }
    void BinaryImporter::TransferFloat(std::wstring key, float& value)
    {
        Read(&value);
    }
    void BinaryImporter::TransferBool(std::wstring key, bool& value)
    {
        Read(&value);
    }
    void BinaryImporter::TransferVector2(std::wstring key, Vector2& value)
    {
        Read(&value);
    }
    void BinaryImporter::TransferVector3(std::wstring key, Vector3& value)
    {
        Read(&value);
    }
    void BinaryImporter::TransferVector4(std::wstring key, Vector4& value)
    {
        Read(&value);
    }
    void BinaryImporter::TransferColor(std::wstring key, Color& value)
    {
        Read(&value);
    }
    void BinaryImporter::TransferRect(std::wstring key, Rect& value)
    {
        Read(&value);
    }
    void BinaryImporter::TransferString(std::wstring key, std::wstring& value)
    {
        int size = 0;
        TransferInt({}, size);

        char* data = new char[size];

        TransferBytes({}, data, size);
        value = reinterpret_cast<wchar_t*>(data);
        value.resize(size / 2);

        delete[] data;
    }
    void BinaryImporter::TransferObjectPtr(std::wstring key, ObjectPtrBase& value)
    {
        int instanceID = 0;
        TransferInt({}, instanceID);
        value = Object::FindObjectOfInstanceID(instanceID);
    }
    void BinaryImporter::TransferBytes(std::wstring key, void* source, int size)
    {
        stream.read(static_cast<char*>(source), size);
    }
}
