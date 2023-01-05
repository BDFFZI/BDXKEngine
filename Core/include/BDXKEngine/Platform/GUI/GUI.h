#pragma once
#include "BDXKEngine/Platform/GL/Texture/Texture2D.h"
#include "BDXKEngine/Base/Object/Object.h"
#define IM_VEC2_CLASS_EXTRA ImVec2(BDXKEngine::Vector2 value){x=value.x;y=value.y;} \
operator BDXKEngine::Vector2()const{return{x,y};}
#define IM_VEC4_CLASS_EXTRA ImVec4(BDXKEngine::Color value){x=value.r;y=value.g;z=value.b;w=value.a;} \
operator BDXKEngine::Color()const{return{x,y,z,w};}
#include <imgui/imgui.h>


namespace BDXKEngine
{
    class GUI
    {
    public:
        static ImTextureID GetImTextureID(const ObjectPtr<Texture2D>& texture);

        static bool DragDropSource(const ObjectPtrBase& value); //拖拽时持续触发
        static bool DragDropTarget(ObjectPtrBase& value);
        static bool IsDockTabVisible();
        static void Dropdown(const std::string& name, const std::function<void()>& onGui);

        static void Initialize();
        static void BeginDraw();
        static void EndDraw();
    };
}
