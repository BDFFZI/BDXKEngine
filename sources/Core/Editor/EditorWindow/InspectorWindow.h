#pragma once
#include "Core/Editor/Core/Editor/EditorWindow.h"
#include "Core/Editor/Core/Editor/GUITransferer.h"
#include "Core/Runtime/Base/Object/Transferer/ObjectTransferer.h"

namespace BDXKEditor
{
    using namespace BDXKEngine;

    class InspectorWindow : public EditorWindow, public AwakeEvent
    {
    public:
        const ObjectPtrBase& GetTarget();
        void SetTarget(const ObjectPtrBase& target);

    private:
        ObjectPtrBase target = {};
        ObjectTransferer<GUITransferer> gui = {};
        bool isDebugGui = false;

        void OnAwake() override;
        void OnGUI() override;
    };
    CustomReflection(InspectorWindow)
}
