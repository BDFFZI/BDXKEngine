#pragma once
#include "Core/Editor.h"

namespace BDXKEditor
{
    class ComponentEditor : public Editor
    {
    public:
        static Editor* EditorFallback(const Reflective& reflective);
        CustomEditorFallback(EditorFallback)

        void OnInspectorGUI() override;
    private:
    };
}
