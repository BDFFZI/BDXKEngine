#include "BinaryExporter.h"
#include "BDXKEngine/Base/Object/ObjectPtrBase.h"

namespace BDXKEngine
{
    BinaryExporter::BinaryExporter(std::iostream& stream) : stream(stream)
    {
    }

    void BinaryExporter::TransferInt(std::string key, int& value)
    {
        Write(value);
    }
    void BinaryExporter::TransferFloat(std::string key, float& value)
    {
        Write(value);
    }
    void BinaryExporter::TransferBool(std::string key, bool& value)
    {
        Write(value);
    }

    void BinaryExporter::TransferVector2(std::string key, Vector2& value)
    {
        Write(value);
    }
    void BinaryExporter::TransferVector3(std::string key, Vector3& value)
    {
        Write(value);
    }
    void BinaryExporter::TransferVector4(std::string key, Vector4& value)
    {
        Write(value);
    }
    void BinaryExporter::TransferColor(std::string key, Color& value)
    {
        Write(value);
    }
    void BinaryExporter::TransferRect(std::string key, Rect& value)
    {
        Write(value);
    }
    void BinaryExporter::TransferString(std::string key, std::string& value)
    {
        int size = static_cast<int>(value.size() * sizeof(char));
        char* data = value.data();

        TransferInt({}, size);
        TransferBytes({}, data, size);
    }
    void BinaryExporter::TransferObjectPtr(std::string key, ObjectPtrBase& value)
    {
        int instanceID = value.GetInstanceID();
        TransferInt({}, instanceID);
    }

    void BinaryExporter::TransferBytes(std::string key, void* source, int size)
    {
        stream.write(static_cast<char*>(source), size);
    }
}
