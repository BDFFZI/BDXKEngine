#pragma once
#include "Core/Runtime/Framework/Actor/GameObject.h"
#include "Core/Editor/Core/Editor/EditorMenu.h"

namespace BDXKEditor
{
    using namespace BDXKEngine;

#define CustomCreateGameObjectMenu(menuItemName,funcName) CustomMenu("GameObject/Create/" menuItemName,funcName)

    class GameObjectMenu
    {
    public:
        static ObjectPtr<GameObject> NewGameObject(const char* name);

        static ObjectPtr<GameObject> CreateGameObject();
        static void ClearParent();

    private:
        CustomCreateGameObjectMenu("Empty", CreateGameObject)
        CustomMenu("GameObject/Clear Parent", ClearParent)
    };
}
