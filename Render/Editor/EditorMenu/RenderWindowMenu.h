#pragma once
#include "Core/Editor/Core/Editor/EditorMenu.h"

namespace BDXKEditor
{
    using namespace BDXKEngine;

    class RenderWindowMenu
    {
    public:
        static void CreateRenderSettingsWindow();
        static void CreateGameWindow();

    private:
        CustomMenu("Window/Scene/RenderSettings", CreateRenderSettingsWindow)
        CustomMenu("Window/Editor/Game", CreateGameWindow)
    };
}
