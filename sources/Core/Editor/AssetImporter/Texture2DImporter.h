#pragma once
#include "Core/Editor/Core/Assets/AssetImporter.h"

namespace BDXKEditor
{
    class Texture2DImporter : public AssetImporter
    {
        ObjectPtrBase Import() override;
    };

    CustomReflection(Texture2DImporter)
    CustomAssetsImporter(png, Texture2DImporter)
}
