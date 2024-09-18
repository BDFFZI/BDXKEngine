#pragma once
#include "Core/Runtime/Platform/GL/Texture/Texture2D.h"
#include "Core/Runtime/Base/Object/Object.h"
#define IM_VEC2_CLASS_EXTRA ImVec2(BDXKEngine::Vector2 value){x=value.x;y=value.y;} \
operator BDXKEngine::Vector2()const{return{x,y};}
#define IM_VEC4_CLASS_EXTRA ImVec4(BDXKEngine::Color value){x=value.r;y=value.g;z=value.b;w=value.a;} \
operator BDXKEngine::Color()const{return{x,y,z,w};}
#include <imgui.h>
#include <GLFW/glfw3.h>

namespace BDXKEngine
{
    class GUI
    {
    public:
        static ImTextureID GetImTextureID(const ObjectPtr<Texture2D>& texture);

        static bool IsDragSource(const ObjectPtrBase& value); //拖拽时持续触发
        static bool IsDragTarget(ObjectPtrBase& value);
        static bool IsDockTabVisible();
        static void Dropdown(const std::string& name, const std::function<void()>& onGui);
        static bool ButtonWindow(); //请作为第一个UI渲染

        static void Initialize(GLFWwindow* glfWwindow);
        static void BeginDraw();
        static void EndDraw();
    };
}
