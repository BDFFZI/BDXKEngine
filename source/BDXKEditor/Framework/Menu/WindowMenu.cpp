#include "WindowMenu.h"
#include "BDXKEngine/Framework/Physics/PhysicsSettings.h"
#include "BDXKEngine/Framework/Renderer/Core/RenderSettings.h"
#include "BDXKEditor/Framework/EditorWindow/GameWindow.h"
#include "BDXKEditor/Framework/EditorWindow/InspectorWindow.h"
#include "BDXKEditor/Framework/EditorWindow/Core/EditorWindow.h"

namespace BDXKEditor
{
    void WindowMenu::CreateRenderSettingsWindow()
    {
        const ObjectPtr<InspectorWindow> inspector = EditorWindow::Create<InspectorWindow>();
        inspector->SetTarget(RenderSettings::GetSingleton());
        inspector->Show();
    }
    void WindowMenu::CreateRenderPhysicsSettings()
    {
        const ObjectPtr<InspectorWindow> inspector = EditorWindow::Create<InspectorWindow>();
        inspector->SetTarget(PhysicsSettings::GetSingleton());
        inspector->Show();
    }
    void WindowMenu::CreateGameWindow()
    {
        EditorWindow::Create<GameWindow>()->Show();
    }
}
