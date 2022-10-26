#include "EditorSystem.h"
#include "Tools/CameraController.h"
#include "View/EditorWindow.h"
#include "View/SceneView.h"
#include "View/HierarchyView.h"
#include "View/InspectorView.h"

namespace BDXKEditor
{
    ObjectPtr<Scene> EditorSystem::mainScene;
    ObjectPtrBase EditorSystem::dragging;
    ObjectPtrBase EditorSystem::focusing;
    ObjectPtr<SceneView> EditorSystem::sceneView;
    ObjectPtr<HierarchyView> EditorSystem::hierarchyView;
    ObjectPtr<InspectorView> EditorSystem::inspectorView;
    
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
    const ObjectPtr<SceneView>& EditorSystem::GetSceneView()
    {
        return sceneView;
    }
    const ObjectPtr<HierarchyView>& EditorSystem::GetHierarchyView()
    {
        return hierarchyView;
    }
    const ObjectPtr<InspectorView>& EditorSystem::GetInspectorView()
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
        SetCreator<SceneView>();
        SetCreator<HierarchyView>();
        SetCreator<InspectorView>();
        SetCreator<CameraController>();

        sceneView = EditorWindow::Create<SceneView>();
        hierarchyView = EditorWindow::Create<HierarchyView>();
        inspectorView = EditorWindow::Create<InspectorView>();

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
