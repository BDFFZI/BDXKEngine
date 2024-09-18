#include "BinaryReader.h"

namespace BDXKEngine
{
    bool BinaryReader::IsImporter()
    {
        return true;
    }
    void BinaryReader::Reset(std::string& data)
    {
        stream.str(data);
    }

    void BinaryReader::ReadStreamTo(char* value, const int size)
    {
        stream.read(value, size);
    }

    void BinaryReader::TransferInt(int& value)
    {
        ReadStreamTo(value);
    }
    void BinaryReader::TransferFloat(float& value)
    {
        ReadStreamTo(value);
    }
    void BinaryReader::TransferBool(bool& value)
    {
        ReadStreamTo(value);
    }
    void BinaryReader::TransferString(std::string& value)
    {
        int size = 0;
        ReadStreamTo(size);
        value.resize(size);
        ReadStreamTo(value.data(), size);
    }
    void BinaryReader::TransferBytes(std::vector<char>& value)
    {
        int size = 0;
        ReadStreamTo(size);
        value.resize(size);
        ReadStreamTo(value.data(), size);
    }
    void BinaryReader::TransferGuid(xg::Guid& guid)
    {
        std::string str;
        TransferString(str);
        guid = xg::Guid(str);
    }
    void BinaryReader::TransferVector2(Vector2& value)
    {
        ReadStreamTo(value);
    }
    void BinaryReader::TransferVector3(Vector3& value)
    {
        ReadStreamTo(value);
    }
    void BinaryReader::TransferVector4(Vector4& value)
    {
        ReadStreamTo(value);
    }
    void BinaryReader::TransferMatrix4x4(Matrix4x4& value)
    {
        ReadStreamTo(value);
    }
    void BinaryReader::TransferColor(Color& value)
    {
        ReadStreamTo(value);
    }
    void BinaryReader::TransferRect(Rect& value)
    {
        ReadStreamTo(value);
    }

    BinaryReader::BinaryReader()
    {
        SetTransfer<int>([this](int& value) { TransferInt(value); });
        SetTransfer<float>([this](float& value) { TransferFloat(value); });
        SetTransfer<bool>([this](bool& value) { TransferBool(value); });
        SetTransfer<std::string>([this](std::string& value) { TransferString(value); });
        SetTransfer<std::vector<char>>([this](std::vector<char>& value) { TransferBytes(value); });
        SetTransfer<xg::Guid>([this](xg::Guid& value) { TransferGuid(value); });
        SetTransfer<Vector2>([this](Vector2& value) { TransferVector2(value); });
        SetTransfer<Vector3>([this](Vector3& value) { TransferVector3(value); });
        SetTransfer<Vector4>([this](Vector4& value) { TransferVector4(value); });
        SetTransfer<Matrix4x4>([this](Matrix4x4& value) { TransferMatrix4x4(value); });
        SetTransfer<Color>([this](Color& value) { TransferColor(value); });
        SetTransfer<Rect>([this](Rect& value) { TransferRect(value); });

        SetTransfer<unsigned int>([this](unsigned int& value) { ReadStreamTo(value); });
        SetTransfer<size_t>([this](size_t& value) { ReadStreamTo(value); });
    }
}
