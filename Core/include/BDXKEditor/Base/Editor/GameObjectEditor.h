#pragma once
#include "BDXKEngine/Framework/Core/GameObject.h"
#include "Core/Editor.h"

namespace BDXKEditor
{
    class GameObjectEditor : public Editor
    {
    protected:
        bool DrawTitle(ScriptableObject* target) const;
        void OnInspectorGUI() const override;
    };

    CustomEditor(GameObject, GameObjectEditor)
}
