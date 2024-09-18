#pragma once
#include "Core/Editor/Core/Editor/EditorWindow.h"

namespace BDXKEditor
{
    class GameWindow : public EditorWindow
    {
        static std::unordered_map<ID, bool> isWindowHovered;

        void OnGUI() override;
    };
    CustomReflection(GameWindow)
}
