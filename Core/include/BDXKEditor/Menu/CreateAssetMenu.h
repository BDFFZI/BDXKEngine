#pragma once
#include "Core/Menu.h"
#include "BDXKEngine/Base/Object/Object.h"
#include "BDXKEngine/Framework/Physics/Material/PhysicMaterial.h"
#include "BDXKEngine/Function/Graphics/Material.h"

namespace BDXKEditor
{
    using namespace BDXKEngine;

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
    private:
        inline static std::unordered_map<std::string, std::string> assetNames = {};
    };

    struct CustomCreateAssetMenuRegister
    {
        CustomCreateAssetMenuRegister(const char* funcName, const std::function<ObjectPtrBase()>& func, const char* assetName)
        {
            CreateAssetMenu::AddMenuItem(funcName, func, assetName);
        }
    };

#define CustomCreateAssetMenu(funcName,assetName) inline CustomCreateAssetMenuRegister CustomCreateAssetMenu##funcName = {#funcName,funcName,assetName};


    ObjectPtr<Material> CreateStandardMaterial();
    ObjectPtr<Material> CreateSkyboxMaterial();
    ObjectPtr<PhysicMaterial> CreatePhysicMaterial();

    CustomCreateAssetMenu(CreateStandardMaterial, "Standard.material")
    CustomCreateAssetMenu(CreateSkyboxMaterial, "Skybox.material")
    CustomCreateAssetMenu(CreatePhysicMaterial, "Material.physicMaterial")
}
