#pragma once
#include "Core/EditorWindow.h"

namespace BDXKEditor
{
    class GameWindow : public EditorWindow
    {
        static std::unordered_map<ID, bool> isWindowHovered;

        void OnGUI() override;
    };
}
