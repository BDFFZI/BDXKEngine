﻿#include "GUI.h"
#include <filesystem>
#include <imgui_impl_dx11.h>
#include <imgui_impl_glfw.h>
#include <ImGuizmo.h>
#include <imgui_internal.h>

namespace BDXKEngine
{
    bool GUI::IsDragSource(const ObjectPtrBase& value)
    {
        if (ImGui::BeginDragDropSource())
        {
            ImGui::SetDragDropPayload("Dragging", &value, sizeof(ObjectPtrBase));
            ImGui::EndDragDropSource();
            return true;
        }

        return false;
    }
    bool GUI::IsDragTarget(ObjectPtrBase& value)
    {
        if (ImGui::BeginDragDropTarget())
        {
            if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("Dragging"))
            {
                const ObjectPtrBase& dropping = *static_cast<ObjectPtrBase*>(payload->Data);
                const Reflection& droppingReflection = Reflection::GetReflection(dropping.GetObjectType());
                if (droppingReflection.CanConvertTo(value.GetObjectType()))
                {
                    value = dropping;
                    return true;
                }
            }
            ImGui::EndDragDropTarget();
        }

        return false;
    }
    bool GUI::ButtonWindow()
    {
        const Vector2 cursorPos = ImGui::GetCursorPos();
        ImGui::PushStyleColor(ImGuiCol_Button, {0, 0, 0, 0});
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, {0, 0, 0, 0});
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, {0, 0, 0, 0});
        ImGui::SetNextItemAllowOverlap();
        const bool isClick = ImGui::ButtonEx(
            std::format("##{}", ImGui::GetCurrentWindow()->Name).c_str(),
            ImGui::GetContentRegionAvail()
        );
        ImGui::PopStyleColor();
        ImGui::PopStyleColor();
        ImGui::PopStyleColor();
        ImGui::SetItemAllowOverlap();
        ImGui::SetCursorPos(cursorPos);

        return isClick;
    }
    ImTextureID GUI::GetImTextureID(const ObjectPtr<Texture2D>& texture)
    {
        return texture->GetShaderResourceView();
    }
    bool GUI::IsDockTabVisible()
    {
        return ImGui::GetCurrentWindow()->DockTabIsVisible;
    }
    void GUI::Dropdown(const std::string& name, const std::function<void()>& onGui)
    {
        static bool dropdownOpening = false;

        if (ImGui::Button(name.c_str(), {ImGui::GetContentRegionAvail().x, 0}))dropdownOpening = true;
        if (dropdownOpening && ImGui::BeginListBox(("##" + name).c_str(), {ImGui::GetContentRegionAvail().x, ImGui::GetWindowHeight() / 3}))
        {
            onGui();

            ImGui::EndListBox();
            if (ImGui::IsWindowHovered(ImGuiHoveredFlags_RootAndChildWindows | ImGuiHoveredFlags_AllowWhenBlockedByActiveItem) == false)
                dropdownOpening = false;
        }
    }

    void GUI::Initialize(GLFWwindow* glfWwindow)
    {
        //Imgui初始化
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGui::StyleColorsDark();
        ImGuiIO& io = ImGui::GetIO();
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable | ImGuiConfigFlags_NoMouseCursorChange;
        // io.IniFilename = nullptr;
        // 中文支持
        io.Fonts->AddFontFromFileTTF(
            "C:/Windows/Fonts/SIMYOU.TTF", 18.0f, nullptr,
            io.Fonts->GetGlyphRangesChineseSimplifiedCommon()
        );

        //Imgui后端初始化
        ImGui_ImplGlfw_InitForOther(glfWwindow, true);
        ImGui_ImplDX11_Init(GL::GetDevice(), GL::GetDeviceContext());

        //Imguizmo初始化
        ImGuizmo::AllowAxisFlip(false);
        ImGuizmo::SetGizmoSizeClipSpace(0.2f);
    }
    // void GUI::UnInitialize() //反正软件退出了，操作系统会自动回收吧
    // {
    //     ImGui_ImplDX11_Shutdown();
    //     ImGui_ImplGlfw_Shutdown();
    //
    //     ImGui::DestroyContext();
    // }

    void GUI::BeginDraw()
    {
        ImGui_ImplDX11_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        ImGuizmo::BeginFrame();


        const ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->Pos);
        ImGui::SetNextWindowSize(viewport->Size);
        ImGui::DockSpaceOverViewport(0, nullptr, ImGuiDockNodeFlags_PassthruCentralNode);
        // ImGui::Begin(
        //     "Background", nullptr,
        //     ImGuiWindowFlags_NoDecoration |
        //     ImGuiWindowFlags_NoSavedSettings |
        //     ImGuiWindowFlags_NoBackground |
        //     ImGuiWindowFlags_NoMove
        // );
    }
    void GUI::EndDraw()
    {
        //ImGui::End();
        ImGui::Render();
        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
    }
}
