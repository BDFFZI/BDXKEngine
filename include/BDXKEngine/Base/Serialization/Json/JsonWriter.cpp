#include "JsonWriter.h"

#include "BDXKEngine/Base/Serialization/ISerializable.h"

namespace BDXKEngine
{
    JsonWriter::JsonWriter(rapidjson::Document& buffer): JsonTransferrer(buffer)
    {
    }

    TransferDirection JsonWriter::GetTransferDirection()
    {
        return TransferDirection::Output;
    }

    void JsonWriter::TransferValue(int& value)
    {
        GetCurrentNode().SetInt(value);
    }
    void JsonWriter::TransferValue(float& value)
    {
        GetCurrentNode().SetFloat(value);
    }
    void JsonWriter::TransferValue(bool& value)
    {
        GetCurrentNode().SetBool(value);
    }
    void JsonWriter::TransferValue(Vector2& value)
    {
      // auto& array = GetCurrentNode().SetArray();
      //   array.PushBack()
      //   GetCurrentNode()[0].SetFloat(value.x);
      //   GetCurrentNode()[1].SetFloat(value.y);
    }
    void JsonWriter::TransferValue(Vector3& value)
    {
        GetCurrentNode()[0].SetFloat(value.x);
        GetCurrentNode()[1].SetFloat(value.y);
        GetCurrentNode()[2].SetFloat(value.z);
    }
    void JsonWriter::TransferValue(Vector4& value)
    {
        GetCurrentNode()[0].SetFloat(value.x);
        GetCurrentNode()[1].SetFloat(value.y);
        GetCurrentNode()[2].SetFloat(value.z);
        GetCurrentNode()[3].SetFloat(value.w);
    }
    void JsonWriter::TransferValue(Color& value)
    {
        GetCurrentNode()[0].SetFloat(value.r);
        GetCurrentNode()[1].SetFloat(value.g);
        GetCurrentNode()[2].SetFloat(value.b);
        GetCurrentNode()[3].SetFloat(value.a);
    }
    void JsonWriter::TransferValue(Rect& value)
    {
        GetCurrentNode()[0].SetFloat(value.x);
        GetCurrentNode()[1].SetFloat(value.y);
        GetCurrentNode()[2].SetFloat(value.width);
        GetCurrentNode()[3].SetFloat(value.height);
    }
    void JsonWriter::TransferValue(std::string& value)
    {
        GetCurrentNode()[0].SetString(value.c_str(), static_cast<rapidjson::SizeType>(value.size()));
    }
    void JsonWriter::TransferValue(ObjectPtrBase& value)
    {
    }
    void JsonWriter::TransferValue(ISerializable& value)
    {
        value.Transfer(*this);
    }
    void JsonWriter::TransferValue(std::vector<ObjectPtrBase>& vector)
    {
    }
    void JsonWriter::TransferValue(char* source, int size)
    {
    }
}
