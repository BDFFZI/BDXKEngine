#include "EditorSystem.h"
#include "BDXKEditor/EditorWindow/SceneWindow.h"
#include "BDXKEditor/EditorWindow/HierarchyWindow.h"
#include "BDXKEditor/EditorWindow/InspectorWindow.h"
#include "BDXKEditor/Editor/GameObjectEditor.h"

namespace BDXKEditor
{
    std::unordered_map<std::string, std::function<Editor*()>> EditorSystem::editors;
    ObjectPtr<Scene> EditorSystem::mainScene;
    ObjectPtrBase EditorSystem::dragging;
    ObjectPtrBase EditorSystem::focusing;
    ObjectPtr<SceneWindow> EditorSystem::sceneView;
    ObjectPtr<HierarchyWindow> EditorSystem::hierarchyView;
    ObjectPtr<InspectorWindow> EditorSystem::inspectorView;

    const ObjectPtr<Scene>& EditorSystem::GetMainScene()
    {
        return mainScene;
    }
    const ObjectPtrBase& EditorSystem::GetDragging()
    {
        return dragging;
    }
    const ObjectPtrBase& EditorSystem::GetFocusing()
    {
        return focusing;
    }
    const ObjectPtr<SceneWindow>& EditorSystem::GetSceneView()
    {
        return sceneView;
    }
    const ObjectPtr<HierarchyWindow>& EditorSystem::GetHierarchyView()
    {
        return hierarchyView;
    }
    const ObjectPtr<InspectorWindow>& EditorSystem::GetInspectorView()
    {
        return inspectorView;
    }

    void EditorSystem::SetMainScene(const ObjectPtr<Scene>& mainScene)
    {
        if (EditorSystem::mainScene.IsNotNull())
        {
            DestroyImmediate(EditorSystem::mainScene->Find("EditorSystem").ToObjectBase());
        }

        SetCreator<EditorSystem>();
        EditorSystem::mainScene = mainScene;
        EditorSystem::mainScene->AddGameObject("EditorSystem")->AddComponent<EditorSystem>();
    }
    void EditorSystem::SetDragging(const ObjectPtrBase& dragging)
    {
        EditorSystem::dragging = dragging;
    }
    void EditorSystem::SetFocusing(const ObjectPtrBase& focusing)
    {
        EditorSystem::focusing = focusing;
    }

    void EditorSystem::OnDrawGUI()
    {
        ImGui::ShowDemoWindow();
    }
    void EditorSystem::OnAwake()
    {
        SetCreator<Editor>();
        SetCreator<SceneWindow>();
        SetCreator<HierarchyWindow>();
        SetCreator<InspectorWindow>();
        SetCreator<CameraController>();
        SetCreator<GameObjectEditor>();
        
        SetEditor<GameObject, GameObjectEditor>();

        sceneView = EditorWindow::Create<SceneWindow>();
        hierarchyView = EditorWindow::Create<HierarchyWindow>();
        inspectorView = EditorWindow::Create<InspectorWindow>();

        sceneView->Show();
        hierarchyView->Show();
        inspectorView->Show();
    }
    void EditorSystem::OnDestroy()
    {
        mainScene = nullptr;
        dragging = nullptr;
        focusing = nullptr;
        sceneView = nullptr;
        hierarchyView = nullptr;
        inspectorView = nullptr;
    }
}
