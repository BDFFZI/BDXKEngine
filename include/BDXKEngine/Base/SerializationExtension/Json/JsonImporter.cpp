#include "JsonImporter.h"

#include "BDXKEngine/Base/Extension/String.h"

namespace BDXKEngine
{
    void JsonImporter::TransferJson(std::string key, std::string& value)
    {
        rapidjson::Document document;
        document.CopyFrom(GetCurrentNode()[key.c_str()], document.GetAllocator());
        Export(document, value);
    }
    void JsonImporter::Reset(std::string& data)
    {
        Import(GetDocument(), data);
        ResetNodes();
    }
    void JsonImporter::PushPath(const std::string& key)
    {
        PushNode(&GetCurrentNode()[key.c_str()]);
    }
    void JsonImporter::PopPath(std::string& key)
    {
        PopNode();
    }

    void JsonImporter::TransferInt(int& value)
    {
        value = GetCurrentNode().GetInt();
    }
    void JsonImporter::TransferFloat(float& value)
    {
        value = GetCurrentNode().GetFloat();
    }
    void JsonImporter::TransferBool(bool& value)
    {
        value = GetCurrentNode().GetBool();
    }
    void JsonImporter::TransferVector2(Vector2& value)
    {
        const auto& array = GetCurrentNode().GetArray();

        value.x = array[0].GetFloat();
        value.y = array[1].GetFloat();
    }
    void JsonImporter::TransferVector3(Vector3& value)
    {
        const auto& array = GetCurrentNode().GetArray();

        value.x = array[0].GetFloat();
        value.y = array[1].GetFloat();
        value.z = array[2].GetFloat();
    }
    void JsonImporter::TransferVector4(Vector4& value)
    {
        const auto& array = GetCurrentNode().GetArray();

        value.x = array[0].GetFloat();
        value.y = array[1].GetFloat();
        value.z = array[2].GetFloat();
        value.w = array[3].GetFloat();
    }
    void JsonImporter::TransferColor(Color& value)
    {
        const auto& array = GetCurrentNode().GetArray();

        value.r = array[0].GetFloat();
        value.g = array[1].GetFloat();
        value.b = array[2].GetFloat();
        value.a = array[3].GetFloat();
    }
    void JsonImporter::TransferRect(Rect& value)
    {
        const auto& array = GetCurrentNode().GetArray();

        value.x = array[0].GetFloat();
        value.y = array[1].GetFloat();
        value.width = array[2].GetFloat();
        value.height = array[3].GetFloat();
    }
    void JsonImporter::TransferString(std::string& value)
    {
        value = GetCurrentNode().GetString();
    }
    void JsonImporter::TransferBytes(std::vector<char>& value)
    {
        std::string base64;
        TransferString(base64);

        value.resize(base64.size() / 4 * 3);
        value.resize(String::DecodingBase64(
                base64.data(),
                static_cast<int>(base64.size()),
                reinterpret_cast<unsigned char*>(value.data()))
        );
    }
}
