#pragma once
#include "Core/Importer.h"

namespace BDXKEditor
{
    class MeshImporter : public Importer
    {
        ObjectPtrBase ImportAsset(const std::string& filePath) override;
    };

    CustomReflection(MeshImporter)
    CustomAssetsImporter(glb, MeshImporter)
}
