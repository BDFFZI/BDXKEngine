#pragma once
#include "BDXKEditor/Editor/Core/Editor.h"
#include "BDXKEngine/Base/Object/Core/ObjectTransferer.h"
#include "Core/EditorWindow.h"

namespace BDXKEditor
{
    using namespace BDXKEngine;

    class InspectorWindow : public EditorWindow, public AwakeHandler
    {
    public:
        void SetTarget(const ObjectPtrBase& target);
    private:
        ObjectPtrBase target;
        ObjectTransferer<GUITransferer> gui;
        bool isSpecial = false;

        void OnAwake() override;
        void OnGUI() override;
    };
}
