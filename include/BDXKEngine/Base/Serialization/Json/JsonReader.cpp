#include "JsonReader.h"

#include "BDXKEngine/Base/Serialization/ISerializable.h"

namespace BDXKEngine
{
    JsonReader::JsonReader(rapidjson::Document& buffer): JsonTransferrer(buffer)
    {
    }

    TransferDirection JsonReader::GetTransferDirection()
    {
        return TransferDirection::Input;
    }
    void JsonReader::PushPath(const std::string& key)
    {
        PushNode(&GetCurrentNode()[key.c_str()]);
    }
    void JsonReader::PopPath(std::string& key)
    {
        PopNode();
    }

    void JsonReader::TransferValue(int& value)
    {
        value = GetCurrentNode().GetInt();
    }
    void JsonReader::TransferValue(float& value)
    {
        value = GetCurrentNode().GetFloat();
    }
    void JsonReader::TransferValue(bool& value)
    {
        value = GetCurrentNode().GetBool();
    }
    void JsonReader::TransferValue(Vector2& value)
    {
        const auto& array = GetCurrentNode().GetArray();

        value.x = array[0].GetFloat();
        value.y = array[1].GetFloat();
    }
    void JsonReader::TransferValue(Vector3& value)
    {
        const auto& array = GetCurrentNode().GetArray();

        value.x = array[0].GetFloat();
        value.y = array[1].GetFloat();
        value.z = array[2].GetFloat();
    }
    void JsonReader::TransferValue(Vector4& value)
    {
        const auto& array = GetCurrentNode().GetArray();

        value.x = array[0].GetFloat();
        value.y = array[1].GetFloat();
        value.z = array[2].GetFloat();
        value.w = array[3].GetFloat();
    }
    void JsonReader::TransferValue(Color& value)
    {
        const auto& array = GetCurrentNode().GetArray();

        value.r = array[0].GetFloat();
        value.g = array[1].GetFloat();
        value.b = array[2].GetFloat();
        value.a = array[3].GetFloat();
    }
    void JsonReader::TransferValue(Rect& value)
    {
        const auto& array = GetCurrentNode().GetArray();

        value.x = array[0].GetFloat();
        value.y = array[1].GetFloat();
        value.width = array[2].GetFloat();
        value.height = array[3].GetFloat();
    }
    void JsonReader::TransferValue(std::string& value)
    {
        value = GetCurrentNode().GetString();
    }
    void JsonReader::TransferValue(ObjectPtrBase& value)
    {
    }
    void JsonReader::TransferValue(ISerializable& value)
    {
        value.Transfer(*this);
    }
    void JsonReader::TransferValue(std::vector<ObjectPtrBase>& vector)
    {
    }
    void JsonReader::TransferValue(char* source, int size)
    {
    }
}
