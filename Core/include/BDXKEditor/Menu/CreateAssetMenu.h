#pragma once
#include "BDXKEditor/Function/Assets.h"
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

#define CustomCreateAssetMenu(menuItemName,func,assetName) inline static CustomCreateAssetMenuRegister CustomCreateAssetMenu##func = {menuItemName,func,assetName};
#define CustomCreateAssetMenu1(func,assetName) inline static CustomCreateAssetMenuRegister CustomCreateAssetMenu##func = {#func,func,assetName};

    class CreateAssetMenu
    {
    public:
        static void AddMenuItem(const std::string& menuItemName, const std::function<ObjectPtrBase()>& func, const std::string& assetName)
        {
            Menu::AddMenuItem("Assets/Create/" + menuItemName, [=]()
            {
                ObjectPtrBase asset = func();
                Assets::Save(assetName, asset);
            });
        }

        static ObjectPtr<Material> CreateStandardMaterial();
        static ObjectPtr<Material> CreateUnlitMaterial();
        static ObjectPtr<Material> CreateSkyboxMaterial();
        static ObjectPtr<PhysicMaterial> CreatePhysicMaterial();
    private:
        inline static std::unordered_map<std::string, std::string> assetNames = {};
        CustomCreateAssetMenu("StandardMaterial", CreateStandardMaterial, "Standard.material")
        CustomCreateAssetMenu("UnlitMaterial", CreateUnlitMaterial, "Unlit.material")
        CustomCreateAssetMenu("SkyboxMaterial", CreateSkyboxMaterial, "Skybox.material")
        CustomCreateAssetMenu("PhysicMaterial", CreatePhysicMaterial, "Material.physicMaterial")
    };
}
