#pragma once
#include "Core/Editor/Core/Assets/AssetImporter.h"

namespace BDXKEditor
{
    class SceneImporter : public AssetImporter
    {
        ObjectPtrBase Import() override;
    };

    CustomReflection(SceneImporter)
    CustomAssetsImporter(scene, SceneImporter)
}
