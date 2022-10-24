#include "IMGUIManager.h"
#include <imgui/imgui_impl_dx11.h>
#include <imgui/imgui_impl_win32.h>
#include "BDXKEngine/Platform/GL/Resources/Texture.h"


extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace BDXKEngine
{
    ImTextureID IMGUIManager::GetImTextureID(ObjectPtr<Texture2D> texture)
    {
        return TextureEditor::GetShaderResourceView(texture.ToObjectPtr<Texture>());
    }
    void IMGUIManager::Initialize(Window* window)
    {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();
        ImGui::StyleColorsDark();

        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
        io.Fonts->AddFontFromFileTTF(
            "C:\\Windows\\Fonts\\simkai.ttf", 18.0f, nullptr,
            io.Fonts->GetGlyphRangesChineseFull()
        );

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
    void IMGUIManager::EndDraw()
    {
        // ImGui::End();
        ImGui::Render();
        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
    }
}
