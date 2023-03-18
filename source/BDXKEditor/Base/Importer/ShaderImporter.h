#pragma once
#include "Core/Importer.h"

namespace BDXKEditor
{
    class ShaderImporter : public Importer
    {
        ObjectPtrBase ImportAsset(const std::string& filePath) override;
    };

    CustomReflection(ShaderImporter)
    CustomAssetsImporter(hlsl, ShaderImporter)
}
