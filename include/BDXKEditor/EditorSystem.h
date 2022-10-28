#pragma once
#include "BDXKEngine/Framework/Component/Behavior/Behavior.h"
#include "BDXKEngine/Framework/Component/Renderer/RenderEvent.h"
#include "BDXKEngine/Framework/Scene/Scene.h"
#include "Editor/Editor.h"

namespace BDXKEditor
{
    class InspectorWindow;
    class HierarchyWindow;
    class SceneWindow;
    using namespace BDXKEngine;

    class EditorSystem : public Behavior, public AwakeHandler, public DestroyHandler, public DrawGUIHandler
    {
    public:
        static ObjectPtr<Editor> GetEditor(const std::string& typeID)
        {
            const auto item = editors.find(typeID);
            const std::unique_ptr<Editor> editor(item == editors.end() ? new Editor() : item->second());
            editor->SetIsEnabling(false);
            return Instantiate(editor.get());
        }
        static const ObjectPtr<Scene>& GetMainScene();
        static const ObjectPtrBase& GetDragging();
        static const ObjectPtrBase& GetFocusing();
        static const ObjectPtr<SceneWindow>& GetSceneView();
        static const ObjectPtr<HierarchyWindow>& GetHierarchyView();
        static const ObjectPtr<InspectorWindow>& GetInspectorView();

        template <typename TObject, typename TEditor>
        static void SetEditor()
        {
            editors[ParseTypeID<TObject>().c_str()] = [] { return new TEditor(); };
        }

        static void SetMainScene(const ObjectPtr<Scene>& mainScene);
        static void SetDragging(const ObjectPtrBase& dragging);
        static void SetFocusing(const ObjectPtrBase& focusing);
    private:
        static std::unordered_map<std::string, std::function<Editor*()>> editors;
        static ObjectPtr<Scene> mainScene;
        static ObjectPtrBase dragging;
        static ObjectPtrBase focusing;
        static ObjectPtr<SceneWindow> sceneView;
        static ObjectPtr<HierarchyWindow> hierarchyView;
        static ObjectPtr<InspectorWindow> inspectorView;

        void OnDrawGUI() override;
        void OnAwake() override;
        void OnDestroy() override;
    };
}
