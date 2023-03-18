#include "AudioClipImporter.h"

#include "BDXKEngine/Platform/Audio/AudioClip/AudioClip.h"
#include "BDXKEngine/Platform/Audio/AudioClip/AudioClipImport.h"

namespace BDXKEditor
{
    ObjectPtrBase AudioClipImporter::ImportAsset(const std::string& filePath)
    {
        return static_cast<ObjectPtrBase>(AudioClipImport::Wav(filePath));
    }
}
