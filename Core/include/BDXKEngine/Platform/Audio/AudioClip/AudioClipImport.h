#pragma once
#include "AudioClip.h"

namespace BDXKEngine
{
    class AudioClipImport
    {
    public:
        static ObjectPtr<AudioClip> Wav(const std::string& fileName);
    };
}
