#pragma once
#include "Platform/Window/Window.h"


namespace BDXKEngine
{
    class GameObjectManager
    {
    protected:
        static GameObjectManager* Initialize(Window* window);
    };
}
