#pragma once
#include "Core/EditorWindow.h"

namespace BDXKEditor
{
    class GameWindow : public EditorWindow
    {
        static std::unordered_map<int, bool> isWindowHovered;

        void OnGUI() override;
    };
}
