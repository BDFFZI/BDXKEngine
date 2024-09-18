#pragma once
#include "Core/Editor/Core/Assets/AssetImporter.h"

namespace BDXKEditor
{
    class ObjectImporter : public AssetImporter
    {
        ObjectPtrBase Import() override;
    };

    CustomReflection(ObjectImporter)
    CustomAssetsImporter(prefab, ObjectImporter)
}
