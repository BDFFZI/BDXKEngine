#include "BinaryImporter.h"
#include "Base/Object/ObjectPtrBase.h"

namespace BDXKEngine
{
    BinaryImporter::BinaryImporter(std::iostream& stream) : stream(stream)
    {
    }

    int BinaryImporter::TransferInt()
    {
        int value = {};
        Read(&value);
        return value;
    }

    float BinaryImporter::TransferFloat()
    {
        float value = {};
        Read(&value);
        return value;
    }

    bool BinaryImporter::TransferBool()
    {
        bool value = {};
        Read(&value);
        return value;
    }

    Vector2 BinaryImporter::TransferVector2()
    {
        Vector2 value = {};
        Read(&value);
        return value;
    }

    Vector3 BinaryImporter::TransferVector3()
    {
        Vector3 value = {};
        Read(&value);
        return value;
    }

    Vector4 BinaryImporter::TransferVector4()
    {
        Vector4 value = {};
        Read(&value);
        return value;
    }

    Color BinaryImporter::TransferColor()
    {
        Color value = {};
        Read(&value);
        return value;
    }

    Rect BinaryImporter::TransferRect()
    {
        Rect value = {};
        Read(&value);
        return value;
    }

    std::wstring BinaryImporter::TransferString()
    {
        const int size = TransferInt();
        char* data = new char[size];
        TransferBytes(data, size);

        std::wstring value = reinterpret_cast<wchar_t*>(data);
        value.resize(size / 2);
        delete[] data;
        return value;
    }

    ObjectPtrBase BinaryImporter::TransferObjectPtr()
    {
        const int instanceID = TransferInt();
        return ObjectPtrBase{Object::FindObjectOfInstanceID(instanceID)};
    }

    void BinaryImporter::TransferBytes(void* source, int size)
    {
        stream.read(reinterpret_cast<char*>(source), size);
    }
}
