#include "RenderWindowMenu.h"
#include "Core/Editor/Core/Editor/EditorWindow.h"
#include "Core/Editor/Core/Settings/Settings.h"
#include "Core/Editor/EditorWindow/InspectorWindow.h"
#include "Render/Editor/AssetImporter/RenderAssets.h"
#include "Render/Editor/EditorWindow/GameWindow.h"
#include "Render/Runtime/Renderer/Core/RenderSetting.h"


namespace BDXKEditor
{
    void RenderWindowMenu::CreateRenderSettingsWindow()
    {
        const ObjectPtr<InspectorWindow> inspector = EditorWindow::Create<InspectorWindow>();
        inspector->SetTarget(Settings::GetSetting<RenderSetting>());
        inspector->Show();
    }

    void RenderWindowMenu::CreateGameWindow()
    {
        EditorWindow::Create<GameWindow>()->Show();
    }
}
