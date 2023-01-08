#pragma once
#include "Core/Editor.h"

namespace BDXKEditor
{
    class ComponentEditor : public Editor
    {
    public:
        static void StaticConstructor();
        CustomStaticConstructor(StaticConstructor)

        void OnInspectorGUI() const override;
    };
}
