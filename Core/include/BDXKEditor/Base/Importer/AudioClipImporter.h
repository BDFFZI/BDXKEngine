#pragma once
#include "Core/Importer.h"

namespace BDXKEditor
{
    class AudioClipImporter : public Importer
    {
        ObjectPtrBase ImportAsset(const std::string& filePath) override;
    };

    CustomReflection(AudioClipImporter)
    CustomAssetsImporter(wav, AudioClipImporter)
}
