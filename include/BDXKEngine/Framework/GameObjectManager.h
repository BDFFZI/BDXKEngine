#pragma once
#include "BDXKEngine/Base/Object/ObjectManager.h"
#include "BDXKEngine/Platform/Window/Window.h"


namespace BDXKEngine
{
    class GameObjectManager : ObjectManager
    {
    protected:
        static GameObjectManager* Initialize(Window* window);
    };
}
