#pragma once
#include "Editor.h"

namespace BDXKEditor
{
    class ComponentEditor : public Editor
    {
    public:
        void OnInspectorGUI() override;
    };
}
