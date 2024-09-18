#include "PackageSetting.h"

#include "Core/Editor/Core/Assets/AssetDatabase.h"
#include "Core/Editor/Core/Settings/Settings.h"

namespace BDXKEditor
{
    std::filesystem::path PackageSetting::GetAssetsPath(const std::string& packageName)
    {
        const auto index = std::ranges::find(packageNames, packageName);
        if (index == packageNames.end())
            return {};
        else
            return packagePaths[index - packageNames.begin()] + "/Assets";
    }
    void PackageSetting::Transfer(Transferer& transferer)
    {
        Object::Transfer(transferer);
        TransferFieldInfo(packageNames);
        TransferFieldInfo(packagePaths);
    }
    void PackageSetting::OnEngineStart()
    {
        Settings::SetEditorSetting<PackageSetting>();

        const size_t count = packageNames.size();
        for (size_t i = 0; i < count; i++)
            AssetDatabase::AddRefreshDir("Packages/" + packageNames[i], packagePaths[i]);
    }
    CustomReflection(PackageSetting)
}
