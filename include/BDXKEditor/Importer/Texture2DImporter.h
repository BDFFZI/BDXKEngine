#pragma once
#include "Core/Importer.h"

namespace BDXKEditor
{
    class Texture2DImporter : public Importer
    {
        ObjectPtrBase Import(std::string filePath) override;
    };

    CustomReflection(Texture2DImporter)
    CustomAssetsImporter(png, Texture2DImporter)
}
