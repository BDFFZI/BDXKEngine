#pragma once
#include "Core/Importer.h"

namespace BDXKEditor
{
    class MeshImporter : public Importer
    {
        ObjectPtrBase ImportObject(std::string filePath) override;
    };

    CustomReflection(MeshImporter)
    CustomAssetsImporter(glb, MeshImporter)
}
