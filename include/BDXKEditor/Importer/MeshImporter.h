#pragma once
#include "Core/Importer.h"

namespace BDXKEditor
{
    class MeshImporter : public Importer
    {
        ObjectPtrBase Import(std::string filePath) override;
    };

    CustomReflection(MeshImporter)
    CustomAssetsImporter(glb, MeshImporter)
}
