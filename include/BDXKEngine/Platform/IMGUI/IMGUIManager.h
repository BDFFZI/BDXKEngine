#pragma once

#include "BDXKEngine/Platform/GL/GL.h"
#include "BDXKEngine/Platform/Window/Window.h"
#include "BDXKEngine/Base/Vector/Vector2.h"
#define IM_VEC2_CLASS_EXTRA ImVec2(BDXKEngine::Vector2 value){x=value.x;y=value.y;} \
operator BDXKEngine::Vector2(){return{x,y};} 
#include <imgui/imgui.h>


namespace BDXKEngine
{
    class IMGUIManager : GL
    {
    public:
        static ImTextureID GetImTextureID(ObjectPtr<Texture2D> texture);
        static void Initialize(Window* window);
        static void BeginDraw();
        static void EndDraw();
    };
}
