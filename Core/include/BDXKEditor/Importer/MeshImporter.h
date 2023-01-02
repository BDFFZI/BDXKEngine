#pragma once
#include "Core/Importer.h"

namespace BDXKEditor
{
    class MeshImporter : public Importer
    {
        ObjectPtrBase ImportAsset(std::string filePath) override;
    };

    CustomReflection(MeshImporter)
    CustomAssetsImporter(glb, MeshImporter)
}
