#include "IMGUIManager.h"
#include <imgui/backends/imgui_impl_dx11.h>
#include <imgui/backends/imgui_impl_win32.h>

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace BDXKEngine
{
    void IMGUIManager::Initialize(Window* window)
    {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGui::GetIO();
        ImGui::StyleColorsDark();

        ImGui_ImplWin32_Init(window->GetHwnd());
        ImGui_ImplDX11_Init(device, context);

        window->AddNativeEvent(ImGui_ImplWin32_WndProcHandler);

        window->AddDestroyEvent([]
        {
            ImGui_ImplDX11_Shutdown();
            ImGui_ImplWin32_Shutdown();
            ImGui::DestroyContext();
        });
    }
    void IMGUIManager::BeginDraw()
    {
        ImGui_ImplDX11_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();
    }
    void IMGUIManager::EndDraw()
    {
        ImGui::Render();
        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
    }
}
