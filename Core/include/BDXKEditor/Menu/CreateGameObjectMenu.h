#pragma once
#include "Core/Menu.h"
#include "BDXKEngine/Framework/Core/GameObject.h"

namespace BDXKEditor
{
    using namespace BDXKEngine;

    class CreateGameObjectMenu : public Menu<ObjectPtr<GameObject>()>
    {
    };

#define CustomCreateGameObjectMenu(funcName) CustomMenu(ObjectPtr<GameObject>(),funcName)
}
