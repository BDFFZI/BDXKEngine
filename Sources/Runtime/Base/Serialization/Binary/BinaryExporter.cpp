#include "BinaryExporter.h"
#include "Base/Object/ObjectPtrBase.h"

namespace BDXKEngine
{
    BinaryExporter::BinaryExporter(std::iostream& stream) : stream(stream)
    {
    }

    void BinaryExporter::TransferInt(std::wstring key, int value)
    {
        Write(value);
    }
    void BinaryExporter::TransferFloat(std::wstring key, float value)
    {
        Write(value);
    }
    void BinaryExporter::TransferBool(std::wstring key, bool value)
    {
        Write(value);
    }

    void BinaryExporter::TransferVector2(std::wstring key, Vector2 value)
    {
        Write(value);
    }
    void BinaryExporter::TransferVector3(std::wstring key, Vector3 value)
    {
        Write(value);
    }
    void BinaryExporter::TransferVector4(std::wstring key, Vector4 value)
    {
        Write(value);
    }
    void BinaryExporter::TransferColor(std::wstring key, Color value)
    {
        Write(value);
    }
    void BinaryExporter::TransferRect(std::wstring key, Rect value)
    {
        Write(value);
    }
    void BinaryExporter::TransferString(std::wstring key, std::wstring value)
    {
        const int size = static_cast<int>(value.size() * sizeof(wchar_t));
        char* data = reinterpret_cast<char*>(value.data());

        TransferInt({}, size);
        TransferBytes({}, data, size);
    }

    void BinaryExporter::TransferBytes(std::wstring key, void* source, int size)
    {
        stream.write(static_cast<char*>(source), size);
    }
    void BinaryExporter::TransferObjectPtr(std::wstring key, const ObjectPtrBase& value)
    {
        TransferInt({}, value.GetInstanceID());
    }
}
