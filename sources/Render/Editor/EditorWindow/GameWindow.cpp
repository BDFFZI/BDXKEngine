#include "GameWindow.h"
#include <algorithm>
#include "Core/Runtime/Function/GUI/GUI.h"
#include "Core/Runtime/Platform/Window/Input.h"
#include "Render/Runtime/RenderRuntime.h"
#include "Render/Runtime/Renderer/Core/Camera.h"

namespace BDXKEditor
{
    std::unordered_map<ID, bool> GameWindow::isWindowHovered = {};

    void GameWindow::OnGUI()
    {
        if (std::ranges::all_of(Camera::GetCameraQueue(), [](Camera* camera) { return camera->GetRenderTarget().IsNotNull(); }))
        {
            ImGui::SetCursorPosY(ImGui::GetContentRegionAvail().y / 2);
            ImGui::Text("No Camera Direct Render To Screen");
            return;
        }

        isWindowHovered[GetObjectID()] = ImGui::IsWindowHovered();
        Input::SetIsEnabling(std::ranges::any_of(isWindowHovered, [](const std::pair<const int, bool>& item) { return item.second; }));

        const Vector2 windowMin = ImGui::GetWindowContentRegionMin();
        const Vector2 windowMax = ImGui::GetWindowContentRegionMax();
        const Vector2 viewSize = windowMax - windowMin;
        const Vector2 canvasSize = RenderRuntime::GetCameraCanvas()->GetSize();
        const float factor = viewSize.x / viewSize.y > canvasSize.x / canvasSize.y
                                 ? viewSize.y / canvasSize.y
                                 : viewSize.x / canvasSize.x;
        ImGui::SetCursorPos(windowMin + viewSize / 2 - canvasSize * factor * 0.5f);
        ImGui::Image(
            GUI::GetImTextureID(RenderRuntime::GetCameraCanvas()),
            canvasSize * factor
        );
    }
}
