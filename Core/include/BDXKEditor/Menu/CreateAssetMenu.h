#pragma once
#include "Core/Menu.h"
#include "BDXKEngine/Base/Object/Object.h"
#include "BDXKEngine/Framework/Physics/Material/PhysicMaterial.h"
#include "BDXKEngine/Function/Graphics/Material.h"

namespace BDXKEditor
{
    using namespace BDXKEngine;

    struct CustomCreateAssetMenuRegister
    {
        CustomCreateAssetMenuRegister(const char* funcName, const std::function<ObjectPtrBase()>& func, const char* assetName);
    };

#define CustomCreateAssetMenu(funcName,assetName) inline static CustomCreateAssetMenuRegister CustomCreateAssetMenu##funcName = {#funcName,funcName,assetName};

    class CreateAssetMenu : public Menu<ObjectPtrBase()>
    {
    public:
        static void AddMenuItem(const std::string& menuItemName, const std::function<ObjectPtrBase()>& func, const std::string& assetName)
        {
            Menu<ObjectPtrBase()>::AddMenuItem(menuItemName, func);
            assetNames[menuItemName] = assetName;
        }
        static const std::string& GetAssetName(const std::string& menuItemName)
        {
            return assetNames[menuItemName];
        }

        static ObjectPtr<Material> CreateStandardMaterial();
        static ObjectPtr<Material> CreateUnlitMaterial();
        static ObjectPtr<Material> CreateSkyboxMaterial();
        static ObjectPtr<PhysicMaterial> CreatePhysicMaterial();
    private:
        inline static std::unordered_map<std::string, std::string> assetNames = {};
        CustomCreateAssetMenu(CreateStandardMaterial, "Standard.material")
        CustomCreateAssetMenu(CreateUnlitMaterial, "Unlit.material")
        CustomCreateAssetMenu(CreateSkyboxMaterial, "Skybox.material")
        CustomCreateAssetMenu(CreatePhysicMaterial, "Material.physicMaterial")
    };
}
