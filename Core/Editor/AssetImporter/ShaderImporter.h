#pragma once
#include "Core/Editor/Core/Assets/AssetImporter.h"

namespace BDXKEditor
{
    class ShaderImporter : public AssetImporter
    {
        ObjectPtrBase Import() override;
    };

    CustomReflection(ShaderImporter)
    CustomAssetsImporter(hlsl, ShaderImporter)
}
