#pragma once
#include "Core/Importer.h"

namespace BDXKEditor
{
    class ObjectImporter : public Importer
    {
        ObjectPtrBase ImportObject(std::string filePath) override;
    };

    CustomReflection(ObjectImporter)
    CustomAssetsImporter(scene, ObjectImporter)
    CustomAssetsImporter(material, ObjectImporter)
    CustomAssetsImporter(prefab, ObjectImporter)
}
