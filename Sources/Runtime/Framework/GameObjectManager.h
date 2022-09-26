#pragma once
#include "Platform/Window/Window.h"


namespace BDXKEngine
{
    class GameObjectManager
    {
        static GameObjectManager* Initialize(Window* window);
    };
}
