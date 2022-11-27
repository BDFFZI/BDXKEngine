#pragma once
#include "GUITransferer.h"
#include "../Core/EditorWindow.h"
#include "BDXKEditor/Editor/Core/Editor.h"
#include "BDXKEngine/Base/Object/ObjectTransferer.h"


namespace BDXKEditor
{
    class Editor;
    using namespace BDXKEngine;


    class InspectorWindow : public EditorWindow, public AwakeHandler
    {
    public:
        void SetTarget(const ObjectPtrBase& target);
    private:
        ObjectPtrBase target;
        ObjectTransferer<GUITransferer> gui;

        void OnAwake() override;
        void OnGUI() override;
    };
}
