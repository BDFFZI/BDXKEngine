#pragma once

#include "Platform/GL/GL.h"
#include "Platform/Window/Window.h"
#include "Base/Vector/Vector2.h"
#define IM_VEC2_CLASS_EXTRA ImVec2(BDXKEngine::Vector2 value){x=value.x;y=value.y;}
#include <imgui/imgui.h>


namespace BDXKEngine
{
    class IMGUIManager : GL
    {
    public:
        static void Initialize(Window* window);
        static void BeginDraw();
        static void EndDraw();
    };
}
