#include "BinaryImporter.h"
#include "Base/Object/ObjectPtrBase.h"

namespace BDXKEngine
{
    BinaryImporter::BinaryImporter(std::iostream& stream) : stream(stream)
    {
    }

    int BinaryImporter::TransferInt(std::wstring key)
    {
        int value = {};
        Read(&value);
        return value;
    }

    float BinaryImporter::TransferFloat(std::wstring key)
    {
        float value = {};
        Read(&value);
        return value;
    }

    bool BinaryImporter::TransferBool(std::wstring key)
    {
        bool value = {};
        Read(&value);
        return value;
    }

    Vector2 BinaryImporter::TransferVector2(std::wstring key)
    {
        Vector2 value = {};
        Read(&value);
        return value;
    }

    Vector3 BinaryImporter::TransferVector3(std::wstring key)
    {
        Vector3 value = {};
        Read(&value);
        return value;
    }

    Vector4 BinaryImporter::TransferVector4(std::wstring key)
    {
        Vector4 value = {};
        Read(&value);
        return value;
    }

    Color BinaryImporter::TransferColor(std::wstring key)
    {
        Color value = {};
        Read(&value);
        return value;
    }

    Rect BinaryImporter::TransferRect(std::wstring key)
    {
        Rect value = {};
        Read(&value);
        return value;
    }

    std::wstring BinaryImporter::TransferString(std::wstring key)
    {
        const int size = TransferInt({});
        char* data = new char[size];
        TransferBytes({}, data, size);

        std::wstring value = reinterpret_cast<wchar_t*>(data);
        value.resize(size / 2);
        delete[] data;
        return value;
    }

    ObjectPtrBase BinaryImporter::TransferObjectPtr(std::wstring key)
    {
        const int instanceID = TransferInt({});
        return ObjectPtrBase{Object::FindObjectOfInstanceID(instanceID)};
    }

    void BinaryImporter::TransferBytes(std::wstring key, void* source, int size)
    {
        stream.read(reinterpret_cast<char*>(source), size);
    }
}
