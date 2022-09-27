#pragma once
#include "Base/Object/ObjectManager.h"
#include "Platform/Window/Window.h"


namespace BDXKEngine
{
    class GameObjectManager : ObjectManager
    {
    protected:
        static GameObjectManager* Initialize(Window* window);
    };
}
