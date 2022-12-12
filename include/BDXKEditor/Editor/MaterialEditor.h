#pragma once
#include "BDXKEngine/Function/Graphics/Material.h"
#include "Core/Editor.h"

namespace BDXKEditor
{
    using namespace BDXKEngine;

    class MaterialEditor : public Editor
    {
    public:
        void OnInspectorGUI() override;
    };

    CustomEditor(Material, MaterialEditor)
}
