#pragma once
#include "BDXKEditor/EditorWindow/InspectorWindow.h"
#include "BDXKEngine/Base/Object/ObjectPtrBase.h"
#include "BDXKEngine/Framework/Object/SwitchableObject.h"


namespace BDXKEditor
{
    using namespace BDXKEngine;

    class Editor : public SwitchableObject
    {
    public:
        void SetTarget(ObjectPtrBase& target);
        void SetGui(transferer& gui);

        virtual void OnInspectorGUI();
        virtual void OnSceneGUI();
    protected:
        ObjectPtrBase target = nullptr;
        transferer* gui = nullptr;
    private:
        void Transfer(transferer& transferer) override;
    };
}
