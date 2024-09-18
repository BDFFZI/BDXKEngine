#pragma once
#include "Render/Runtime/Graphics/Material.h"
#include "Core/Editor/Core/Editor/Editor.h"

namespace BDXKEditor
{
    using namespace BDXKEngine;

    class MaterialEditor : public Editor
    {
    public:
        void OnInspectorGUI() const override;
    };

    CustomEditor(Material, MaterialEditor)
}
