#include "ProjectSettings.h"

#include <filesystem>

#include "BDXKEngine/Base/Reflection/Reflection.h"
#include "BDXKEngine/Base/Serializer/Core/Serializer.h"
#include "BDXKEngine/Platform/Serialization/SerializerExtension/Json/JsonWriter.h"
#include "BDXKEngine/Platform/Serialization/SerializerExtension/Json/JsonReader.h"

namespace BDXKEditor
{
    std::string ProjectSettings::ParsePath(const std::string& fileName)
    {
        return "ProjectSettings/" + fileName;
    }
    Reflective* ProjectSettings::Load(const std::string& fileName)
    {
        if (std::filesystem::exists(ParsePath(fileName)) == false)
            return nullptr;

        const std::string data = ReadFile(ParsePath(fileName));
        const Serializer1<JsonReader, JsonWriter> serializer = {};
        return serializer.Deserialize(data);
    }
    void ProjectSettings::Save(const std::string& fileName, Reflective* settings)
    {
        const Serializer1<JsonReader, JsonWriter> serializer = {};
        const std::string data = serializer.Serialize(settings);
        WriteFile(ParsePath(fileName), data);
    }
    void ProjectSettings::StaticConstructor()
    {
        std::filesystem::create_directory("ProjectSettings");
    }
}
