#pragma once
#include <BDXKEngine/Engine.h>
#include "EditorWindow.h"

namespace BDXKEditor
{
    using namespace BDXKEngine;

    class HierarchyView : public EditorWindow, ObjectManager
    {
        std::unordered_map<int, bool> isSpreads;

        void DrawGameObject(const ObjectPtr<GameObject>& gameObject, float offsetX);
        void OnGUI() override;
    };
}
