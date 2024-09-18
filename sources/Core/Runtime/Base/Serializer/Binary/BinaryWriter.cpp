#include "BinaryWriter.h"

namespace BDXKEngine
{
    bool BinaryWriter::IsImporter()
    {
        return false;
    }
    void BinaryWriter::Reset(std::string& data)
    {
        data = stream.str();
        stream.str("");
    }
    void BinaryWriter::WriteStreamFrom(const char* value, const int size)
    {
        stream.write(value, size);
    }


    void BinaryWriter::TransferInt(int& value)
    {
        WriteStreamFrom(value);
    }
    void BinaryWriter::TransferFloat(float& value)
    {
        WriteStreamFrom(value);
    }
    void BinaryWriter::TransferBool(bool& value)
    {
        WriteStreamFrom(value);
    }
    void BinaryWriter::TransferString(const std::string& value)
    {
        int size = static_cast<int>(value.size());
        const char* data = value.data();

        WriteStreamFrom(size);
        WriteStreamFrom(data, size);
    }
    void BinaryWriter::TransferBytes(const std::vector<char>& value)
    {
        int size = static_cast<int>(value.size());

        WriteStreamFrom(size);
        WriteStreamFrom(value.data(), size);
    }
    void BinaryWriter::TransferGuid(const xg::Guid& guid)
    {
        const std::string str = guid.str();
        TransferString(str);
    }
    void BinaryWriter::TransferVector2(Vector2& value)
    {
        WriteStreamFrom(value);
    }
    void BinaryWriter::TransferVector3(Vector3& value)
    {
        WriteStreamFrom(value);
    }
    void BinaryWriter::TransferVector4(Vector4& value)
    {
        WriteStreamFrom(value);
    }
    void BinaryWriter::TransferMatrix4x4(Matrix4x4& value)
    {
        WriteStreamFrom(value);
    }
    void BinaryWriter::TransferColor(Color& value)
    {
        WriteStreamFrom(value);
    }
    void BinaryWriter::TransferRect(Rect& value)
    {
        WriteStreamFrom(value);
    }

    BinaryWriter::BinaryWriter()
    {
        SetTransfer<int>([this](int& value) { TransferInt(value); });
        SetTransfer<float>([this](float& value) { TransferFloat(value); });
        SetTransfer<bool>([this](bool& value) { TransferBool(value); });
        SetTransfer<std::string>([this](const std::string& value) { TransferString(value); });
        SetTransfer<std::vector<char>>([this](const std::vector<char>& value) { TransferBytes(value); });
        SetTransfer<xg::Guid>([this](const xg::Guid& value) { TransferGuid(value); });
        SetTransfer<Vector2>([this](Vector2& value) { TransferVector2(value); });
        SetTransfer<Vector3>([this](Vector3& value) { TransferVector3(value); });
        SetTransfer<Vector4>([this](Vector4& value) { TransferVector4(value); });
        SetTransfer<Matrix4x4>([this](Matrix4x4& value) { TransferMatrix4x4(value); });
        SetTransfer<Color>([this](Color& value) { TransferColor(value); });
        SetTransfer<Rect>([this](Rect& value) { TransferRect(value); });

        SetTransfer<unsigned int>([this](unsigned int& value) { WriteStreamFrom(value); });
        SetTransfer<size_t>([this](size_t& value) { WriteStreamFrom(value); });
    }
}
