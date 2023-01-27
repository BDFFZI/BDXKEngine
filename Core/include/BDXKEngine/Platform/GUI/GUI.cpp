#include "GUI.h"

#include <filesystem>
#include <imgui/imgui_impl_dx11.h>
#include <imgui/imgui_impl_win32.h>
#include <ImGuizmo/ImGuizmo.h>

#include "BDXKEngine/Platform/Window/Window.h"
#include "imgui/imgui_internal.h"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

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
        const bool isClick = ImGui::ButtonEx(
            std::format("##{}", ImGui::GetCurrentWindow()->Name).c_str(),
            ImGui::GetContentRegionAvail(),
            ImGuiButtonFlags_AllowItemOverlap
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

    void GUI::Initialize()
    {
        //Imgui初始化
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGui::StyleColorsDark();
        ImGuiIO& io = ImGui::GetIO();
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
        io.IniFilename = nullptr;
        // 中文支持
        // io.Fonts->AddFontFromFileTTF(
        //     "C:/Windows/Fonts/simkai.ttf", 18.0f, nullptr,
        //     io.Fonts->GetGlyphRangesChineseFull()
        // );

        //Imgui后端初始化
        ImGui_ImplWin32_Init(Window::GetHwnd());
        ImGui_ImplDX11_Init(GL::GetDevice(), GL::GetDeviceContext());

        //Imguizmo初始化
        ImGuizmo::AllowAxisFlip(false);
        ImGuizmo::SetGizmoSizeClipSpace(0.2f);

        Window::AddNativeEvent(ImGui_ImplWin32_WndProcHandler);
        Window::AddDestroyEvent([]
        {
            ImGui_ImplDX11_Shutdown();
            ImGui_ImplWin32_Shutdown();

            ImGui::DestroyContext();
        });
    }
    void GUI::BeginDraw()
    {
        ImGui_ImplDX11_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();
        ImGuizmo::BeginFrame();


        const ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->Pos);
        ImGui::SetNextWindowSize(viewport->Size);
        ImGui::DockSpaceOverViewport(nullptr, ImGuiDockNodeFlags_PassthruCentralNode);
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
