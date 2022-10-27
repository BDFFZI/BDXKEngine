#pragma once
#include "BDXKEngine/Framework/Component/Behavior/Behavior.h"
#include "BDXKEngine/Framework/Component/Renderer/RenderEvent.h"
#include "BDXKEngine/Framework/Scene/Scene.h"
#include "BDXKEditor/Base/Editor.h"

namespace BDXKEditor
{
    class InspectorView;
    class HierarchyView;
    class SceneView;
    using namespace BDXKEngine;

    class EditorSystem : public Behavior, public AwakeHandler, public DestroyHandler, public DrawGUIHandler
    {
    public:
        static ObjectPtr<Editor> GetEditor(const std::string& typeID, const ObjectPtrBase& target)
        {
            const auto item = editors.find(typeID);
            const std::unique_ptr<Editor> editor(item == editors.end() ? new Editor() : item->second());
            editor->SetTarget(target);
            return Instantiate(editor.get());
        }
        static const ObjectPtr<Scene>& GetMainScene();
        static const ObjectPtrBase& GetDragging();
        static const ObjectPtrBase& GetFocusing();
        static const ObjectPtr<SceneView>& GetSceneView();
        static const ObjectPtr<HierarchyView>& GetHierarchyView();
        static const ObjectPtr<InspectorView>& GetInspectorView();

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
        static ObjectPtr<SceneView> sceneView;
        static ObjectPtr<HierarchyView> hierarchyView;
        static ObjectPtr<InspectorView> inspectorView;

        void OnDrawGUI() override;
        void OnAwake() override;
        void OnDestroy() override;
    };
}
