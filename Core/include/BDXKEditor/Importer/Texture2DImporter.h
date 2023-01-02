#pragma once
#include "Core/Importer.h"

namespace BDXKEditor
{
    class Texture2DImporter : public Importer
    {
        ObjectPtrBase ImportAsset(std::string filePath) override;
    };

    CustomReflection(Texture2DImporter)
    CustomAssetsImporter(png, Texture2DImporter)
}
