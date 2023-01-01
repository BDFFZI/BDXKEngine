#pragma once
#include "BDXKEngine/Framework/Core/GameObject.h"
#include "Core/Editor.h"

namespace BDXKEditor
{
    class GameObjectEditor : public Editor
    {
    protected:
        void OnInspectorGUI() override;
    };

    CustomEditor(GameObject, GameObjectEditor)
}
