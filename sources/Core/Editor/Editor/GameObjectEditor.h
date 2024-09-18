#pragma once
#include "Core/Runtime/Framework/Actor/GameObject.h"
#include "Core/Editor/Core/Editor/Editor.h"

namespace BDXKEditor
{
    class GameObjectEditor : public Editor
    {
    protected:
        bool DrawTitle(SwitchableObject* target) const;
        void OnInspectorGUI() const override;
    };

    CustomEditor(GameObject, GameObjectEditor)
}
