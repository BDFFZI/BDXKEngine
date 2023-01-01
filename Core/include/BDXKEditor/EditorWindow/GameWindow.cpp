#include "GameWindow.h"
#include "BDXKEngine/Platform/GUI/GUI.h"

namespace BDXKEditor
{
    void GameWindow::OnGUI()
    {
        const Vector2 windowMin = ImGui::GetWindowContentRegionMin();
        const Vector2 windowMax = ImGui::GetWindowContentRegionMax();
        const Vector2 viewSize = windowMax - windowMin;
        const Vector2 canvasSize = RenderEvent::GetCameraCanvas()->GetSize();
        const float factor = viewSize.x / viewSize.y > canvasSize.x / canvasSize.y
                                 ? viewSize.y / canvasSize.y
                                 : viewSize.x / canvasSize.x;
        ImGui::SetCursorPos(windowMin + viewSize / 2 - canvasSize * factor * 0.5f);
        ImGui::Image(
            GUI::GetImTextureID(RenderEvent::GetCameraCanvas()),
            canvasSize * factor
        );
    }
}
