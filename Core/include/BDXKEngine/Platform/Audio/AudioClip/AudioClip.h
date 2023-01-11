#pragma once
#include "BDXKEngine/Base/Object/Object.h"
#include "BDXKEngine/Platform/Audio/Core/Audio.h"

namespace BDXKEngine
{
    class AudioClip : public Object
    {
    public:
        static ObjectPtr<AudioClip> Create(const std::vector<char>& fmt, const std::vector<char>& data);

        WAVEFORMATEX& GetWaveFormatex();
        XAUDIO2_BUFFER& GetBuffer();
    private:
        std::vector<char> fmt;
        std::vector<char> data;

        WAVEFORMATEX waveformatex = {};
        XAUDIO2_BUFFER buffer = {};

        void Transfer(Transferer& transferer) override;
        void Awake() override;
    };

    CustomReflection(AudioClip)
}
