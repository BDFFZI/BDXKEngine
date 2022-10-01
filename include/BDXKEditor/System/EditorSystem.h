#pragma once
#include "BDXKEngine/Framework/Component/Behavior/Behavior.h"
#include "BDXKEngine/Framework/Component/Renderer/RenderEvent.h"
#include "BDXKEngine/Framework/ScriptableObject/ScriptableObjectEvent.h"

namespace BDXKEditor
{
    class InspectorView;
    class HierarchyView;
    class SceneView;
    using namespace BDXKEngine;

    class EditorSystem : public Behavior, public AwakeHandler, public DrawGUIHandler
    {
    public:
        static ObjectPtr<SceneView> sceneView;
        static ObjectPtr<HierarchyView> hierarchyView;
        static ObjectPtr<InspectorView> inspectorView;
    private:
        void OnAwake() override;

        void OnDrawGUI() override;
    };
}
