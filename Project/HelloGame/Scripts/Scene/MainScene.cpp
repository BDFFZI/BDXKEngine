#include "MainScene.h"
#include <imgui/imgui.h>
#include "../System/Text.h"
#include "BDXKEngine/BDXKEngine.h"

namespace HelloGame
{
    void MainScene::OnDrawGUI()
    {
        ImGui::Button(Text::startGame);
        ImGui::Button(Text::store);
        if (ImGui::Button(Text::exitGame))
        {
            Quit();
        }
    }
}
