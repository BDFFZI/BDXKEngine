#pragma once
#include "Core/Importer.h"

namespace BDXKEditor
{
    class ObjectImporter : public Importer
    {
        ObjectPtrBase ImportAsset(std::string filePath) override;
    };

    CustomReflection(ObjectImporter)
    CustomAssetsImporter(scene, ObjectImporter)
    CustomAssetsImporter(material, ObjectImporter)
    CustomAssetsImporter(physicMaterial, ObjectImporter)
    CustomAssetsImporter(prefab, ObjectImporter)
}
