#include "BDXKJsonImporter.h"

namespace BDXKEngine
{
    void BDXKJsonImporter::Reset(std::string& data)
    {
        GetDocument().Parse(data.c_str());
        ResetNodes();
    }
    void BDXKJsonImporter::PushPath(const std::string& key)
    {
        PushNode(&GetCurrentNode()[key.c_str()]);
    }
    void BDXKJsonImporter::PopPath(std::string& key)
    {
        PopNode();
    }

    void BDXKJsonImporter::TransferInt(int& value)
    {
        value = GetCurrentNode().GetInt();
    }
    void BDXKJsonImporter::TransferFloat(float& value)
    {
        value = GetCurrentNode().GetFloat();
    }
    void BDXKJsonImporter::TransferBool(bool& value)
    {
        value = GetCurrentNode().GetBool();
    }
    void BDXKJsonImporter::TransferVector2(Vector2& value)
    {
        const auto& array = GetCurrentNode().GetArray();

        value.x = array[0].GetFloat();
        value.y = array[1].GetFloat();
    }
    void BDXKJsonImporter::TransferVector3(Vector3& value)
    {
        const auto& array = GetCurrentNode().GetArray();

        value.x = array[0].GetFloat();
        value.y = array[1].GetFloat();
        value.z = array[2].GetFloat();
    }
    void BDXKJsonImporter::TransferVector4(Vector4& value)
    {
        const auto& array = GetCurrentNode().GetArray();

        value.x = array[0].GetFloat();
        value.y = array[1].GetFloat();
        value.z = array[2].GetFloat();
        value.w = array[3].GetFloat();
    }
    void BDXKJsonImporter::TransferColor(Color& value)
    {
        const auto& array = GetCurrentNode().GetArray();

        value.r = array[0].GetFloat();
        value.g = array[1].GetFloat();
        value.b = array[2].GetFloat();
        value.a = array[3].GetFloat();
    }
    void BDXKJsonImporter::TransferRect(Rect& value)
    {
        const auto& array = GetCurrentNode().GetArray();

        value.x = array[0].GetFloat();
        value.y = array[1].GetFloat();
        value.width = array[2].GetFloat();
        value.height = array[3].GetFloat();
    }
    void BDXKJsonImporter::TransferString(std::string& value)
    {
        value = GetCurrentNode().GetString();
    }
}
