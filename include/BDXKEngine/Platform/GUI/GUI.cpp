#include "GUI.h"
#include <imgui/imgui_impl_dx11.h>
#include <imgui/imgui_impl_win32.h>
#include <ImGuizmo/ImGuizmo.h>

#include "imgui/imgui_internal.h"


extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace BDXKEngine
{
    bool GUI::BeginDragDrop(const ObjectPtrBase& value)
    {
        if (ImGui::BeginDragDropSource())
        {
            ImGui::SetDragDropPayload("Dragging", &value, sizeof(ObjectPtrBase));
            ImGui::EndDragDropSource();
            return true;
        }

        return false;
    }
    bool GUI::EndDragDrop(ObjectPtrBase& value)
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
    ImTextureID GUI::GetImTextureID(const ObjectPtr<Texture2D>& texture)
    {
        return texture->GetShaderResourceView();
    }
    bool GUI::IsDockTabVisible()
    {
        return ImGui::GetCurrentWindow()->DockTabIsVisible;
    }

    void GUI::Initialize(Window* window)
    {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGui::StyleColorsDark();
        ImGui_ImplWin32_Init(window->GetHwnd());
        ImGui_ImplDX11_Init(GL::GetDevice(), GL::GetDeviceContext());

        ImGuiIO& io = ImGui::GetIO();
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
        // 中文支持
        // io.Fonts->AddFontFromFileTTF(
        //     "C:/Windows/Fonts/simkai.ttf", 18.0f, nullptr,
        //     io.Fonts->GetGlyphRangesChineseFull()
        // );

        ImGuizmo::AllowAxisFlip(false);
        ImGuizmo::SetGizmoSizeClipSpace(0.2f);

        window->AddNativeEvent(ImGui_ImplWin32_WndProcHandler);

        window->AddDestroyEvent([]
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
        // ImGui::End();
        ImGui::Render();
        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
    }
}
