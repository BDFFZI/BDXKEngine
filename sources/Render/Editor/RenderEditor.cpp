#include "RenderEditor.h"
#include "AssetImporter/RenderAssets.h"
#include "Core/Editor/CoreEditor.h"
#include "Core/Editor/Core/Editor/EditorWindow.h"
#include "Core/Editor/Core/Settings/Settings.h"
#include "EditorWindow/GameWindow.h"
#include "EditorWindow/SceneWindow.h"

namespace BDXKEditor
{
    void RenderEditor::OnEngineStart()
    {
        Settings::SetRuntimeSetting<RenderSetting>();

        const auto sceneWindow = EditorWindow::Create<SceneWindow>(false);
        const auto gameWindow = EditorWindow::Create<GameWindow>(false);
        sceneWindow->Show();
        gameWindow->Show();

        //两个游戏窗口便于观看
        const auto previewWindow = EditorWindow::Create<GameWindow>();
        previewWindow->SetName("PreviewWindow");
        previewWindow->Show();

        CoreEditor::AddClickObjectEvent([=](const ObjectPtrBase& object)
        {
            if (const GameObject* gameObject = object.ToObject<GameObject>(); gameObject != nullptr)
                sceneWindow->SetTarget(gameObject);
        });
    }
    void RenderEditor::OnEngineUpdate()
    {
    }
}
