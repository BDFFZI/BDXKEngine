#pragma once
#include "Core/Importer.h"

namespace BDXKEditor
{
    class ShaderImporter : public Importer
    {
        ObjectPtrBase ImportObject(std::string filePath) override;
    };

    CustomReflection(ShaderImporter)
    CustomAssetsImporter(hlsl, ShaderImporter)
}
