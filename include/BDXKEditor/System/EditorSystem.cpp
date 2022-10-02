#include "EditorSystem.h"

#include "Tools/CameraController.h"
#include "View/EditorWindow.h"
#include "View/SceneView.h"
#include "View/HierarchyView.h"
#include "View/InspectorView.h"

namespace BDXKEditor
{
    ObjectPtr<SceneView> EditorSystem::sceneView;
    ObjectPtr<HierarchyView> EditorSystem::hierarchyView;
    ObjectPtr<InspectorView> EditorSystem::inspectorView;
    ObjectPtrBase EditorSystem::dragging;
    ObjectPtrBase EditorSystem::focusing;

    void EditorSystem::OnAwake()
    {
        AddSerializationInfo<SceneView>();
        AddSerializationInfo<HierarchyView>();
        AddSerializationInfo<InspectorView>();
        AddSerializationInfo<CameraController>();

        sceneView = EditorWindow::Create<SceneView>();
        hierarchyView = EditorWindow::Create<HierarchyView>();
        inspectorView = EditorWindow::Create<InspectorView>();

        sceneView->Show();
        hierarchyView->Show();
        inspectorView->Show();
    }

    void EditorSystem::OnDrawGUI()
    {
        ImGui::ShowDemoWindow();
    }
}
