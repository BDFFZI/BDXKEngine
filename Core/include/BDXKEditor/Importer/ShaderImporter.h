#pragma once
#include "Core/Importer.h"

namespace BDXKEditor
{
    class ShaderImporter : public Importer
    {
        ObjectPtrBase ImportAsset(std::string filePath) override;
    };

    CustomReflection(ShaderImporter)
    CustomAssetsImporter(hlsl, ShaderImporter)
}
