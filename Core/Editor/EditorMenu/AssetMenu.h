#pragma once
#include "Core/Editor/Core/Assets/AssetDatabase.h"
#include "Core/Editor/Core/Editor/EditorMenu.h"

namespace BDXKEditor
{
    using namespace BDXKEngine;

    struct CustomCreateAssetMenuRegister
    {
        CustomCreateAssetMenuRegister(const char* funcName, const std::function<ObjectPtrBase()>& func, const char* assetName)
        {
            EditorMenu::AddMenuItem("Assets/Create/" + std::string(funcName), [=]()
            {
                const ObjectPtrBase asset = func();
                AssetDatabase::Save("Assets/" + std::string(assetName), asset);
            });
        }
    };

    //创建资源菜单
#define CustomCreateAssetMenu(menuItemName,func,assetName) inline static CustomCreateAssetMenuRegister CustomCreateAssetMenu##func = {menuItemName,func,assetName};
#define CustomCreateAssetMenu1(func,assetName) inline static CustomCreateAssetMenuRegister CustomCreateAssetMenu##func = {#func,func,assetName};
}
