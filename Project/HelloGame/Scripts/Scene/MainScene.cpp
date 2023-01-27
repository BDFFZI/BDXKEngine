#include "MainScene.h"
#include <imgui/imgui.h>
#include "../System/Text.h"
#include "BDXKEngine/BDXKEngine.h"
#include "BDXKEngine/Framework/Scene.h"

namespace HelloGame
{
    void MainScene::OnDrawGUI()
    {
        if(ImGui::Button(Text::startGame))
        {
            Scene::Load("Game.scene");
        }
        ImGui::Button(Text::store);
        if (ImGui::Button(Text::exitGame))
        {
            Quit();
        }
    }
}
