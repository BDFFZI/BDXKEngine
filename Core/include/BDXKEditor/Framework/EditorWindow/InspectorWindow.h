#pragma once
#include "Core/EditorWindow.h"
#include "BDXKEngine/Base/Object/Transferer/ObjectTransferer.h"
#include "BDXKEditor/Base/Editor/Core/Editor.h"

namespace BDXKEditor
{
    using namespace BDXKEngine;

    class InspectorWindow : public EditorWindow, public AwakeHandler
    {
    public:
        const ObjectPtrBase& GetTarget();
        void SetTarget(const ObjectPtrBase& target);
    private:
        ObjectPtrBase target = {};
        ObjectTransferer<GUITransferer> gui = {};

        void OnAwake() override;
        void OnGUI() override;
    };
}
