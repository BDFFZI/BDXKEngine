#pragma once
#include "Editor.h"
#include "BDXKEngine/Base/Object/ObjectPtr.h"

namespace BDXKEditor
{
    class GameObjectEditor : public Editor
    {
    public:
        void OnInspectorGUI() override;
    private:
        std::unordered_map<int, ObjectPtr<Editor>> editors;
    };
}
