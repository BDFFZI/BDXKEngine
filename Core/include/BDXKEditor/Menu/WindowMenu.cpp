#include "WindowMenu.h"
#include "BDXKEditor/EditorWindow/GameWindow.h"
#include "BDXKEditor/EditorWindow/InspectorWindow.h"
#include "BDXKEditor/EditorWindow/Core/EditorWindow.h"
#include "BDXKEngine/Framework/Physics/PhysicsSettings.h"
#include "BDXKEngine/Framework/Renderer/Core/RenderSettings.h"

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
