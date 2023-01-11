#include "JsonReader.h"

#include "BDXKEngine/Base/Data/String/String.h"

namespace BDXKEngine
{
    void JsonReader::TransferJson(std::string key, std::string& value)
    {
        rapidjson::Document document;
        document.CopyFrom(GetCurrentNode()[key.c_str()], document.GetAllocator());
        Export(document, value);
    }
    void JsonReader::TransferValue(void* value, const Type& type)
    {
        if (HasCurrentNode())
            JsonTransferer::TransferValue(value, type);
    }

    bool JsonReader::IsImporter()
    {
        return true;
    }
    void JsonReader::Reset(std::string& data)
    {
        Import(GetDocument(), data);
        ResetNodes();
    }
    void JsonReader::PushPath(const std::string& key)
    {
        if (GetCurrentNode().HasMember(key.c_str()) == false)
            PushNode(nullptr);
        else
            PushNode(&GetCurrentNode()[key.c_str()]);
    }
    void JsonReader::PopPath(std::string& key)
    {
        PopNode();
    }


    void JsonReader::TransferInt(int& value)
    {
        value = GetCurrentNode().GetInt();
    }
    void JsonReader::TransferFloat(float& value)
    {
        value = GetCurrentNode().GetFloat();
    }
    void JsonReader::TransferBool(bool& value)
    {
        value = GetCurrentNode().GetBool();
    }
    void JsonReader::TransferVector2(Vector2& value)
    {
        const auto& array = GetCurrentNode().GetArray();

        value.x = array[0].GetFloat();
        value.y = array[1].GetFloat();
    }
    void JsonReader::TransferVector3(Vector3& value)
    {
        const auto& array = GetCurrentNode().GetArray();

        value.x = array[0].GetFloat();
        value.y = array[1].GetFloat();
        value.z = array[2].GetFloat();
    }
    void JsonReader::TransferVector4(Vector4& value)
    {
        const auto& array = GetCurrentNode().GetArray();

        value.x = array[0].GetFloat();
        value.y = array[1].GetFloat();
        value.z = array[2].GetFloat();
        value.w = array[3].GetFloat();
    }
    void JsonReader::TransferMatrix4x4(Matrix4x4& value)
    {
        const auto& array = GetCurrentNode().GetArray();

        value.m00 = array[0].GetFloat();
        value.m10 = array[1].GetFloat();
        value.m20 = array[2].GetFloat();
        value.m30 = array[3].GetFloat();

        value.m01 = array[4].GetFloat();
        value.m11 = array[5].GetFloat();
        value.m21 = array[6].GetFloat();
        value.m31 = array[7].GetFloat();

        value.m02 = array[8].GetFloat();
        value.m12 = array[9].GetFloat();
        value.m22 = array[10].GetFloat();
        value.m32 = array[11].GetFloat();

        value.m03 = array[12].GetFloat();
        value.m13 = array[13].GetFloat();
        value.m23 = array[14].GetFloat();
        value.m33 = array[15].GetFloat();
    }
    void JsonReader::TransferColor(Color& value)
    {
        const auto& array = GetCurrentNode().GetArray();

        value.r = array[0].GetFloat();
        value.g = array[1].GetFloat();
        value.b = array[2].GetFloat();
        value.a = array[3].GetFloat();
    }
    void JsonReader::TransferRect(Rect& value)
    {
        const auto& array = GetCurrentNode().GetArray();

        value.x = array[0].GetFloat();
        value.y = array[1].GetFloat();
        value.width = array[2].GetFloat();
        value.height = array[3].GetFloat();
    }
    void JsonReader::TransferString(std::string& value)
    {
        value = GetCurrentNode().GetString();
    }
    void JsonReader::TransferBytes(std::vector<char>& value)
    {
        std::string base64;
        TransferString(base64);

        value.resize(base64.size() / 4 * 3); //base64字节数量一定是4的倍数，4个base64字节还原成3个普通字节
        value.resize(String::DecodingBase64(
                reinterpret_cast<unsigned char*>(base64.data()),
                static_cast<int>(base64.size()),
                reinterpret_cast<unsigned char*>(value.data()))
        );
    }
}
