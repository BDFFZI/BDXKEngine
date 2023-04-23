#include "Settings.h"
#include <filesystem>

namespace BDXKEditor
{
    std::filesystem::path Settings::ParsePath(const std::string& fileName)
    {
        return "Settings/" + fileName;
    }
    void Settings::OnEngineStart()
    {
        AssetDatabase::AddRefreshDir("Settings", "Settings");
    }
    void Settings::OnEngineDestory()
    {
        for (auto& [type, setting] : settings)
        {
            AssetDatabase::Save(ParsePath(GetTypeName(type)).concat(".setting"), setting);
        }
    }
}
