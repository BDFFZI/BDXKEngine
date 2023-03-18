#pragma once
#include "BDXKEngine/Platform/Audio/AudioClip/AudioClip.h"
#include "BDXKEngine/Platform/Audio/Core/Audio.h"

namespace BDXKEngine
{
    class AudioSourceVoice : public IXAudio2VoiceCallback
    {
    public:
        AudioSourceVoice(const ObjectPtr<AudioClip>& audioClip);
        virtual ~AudioSourceVoice();

        void SetVolume(float volume) const;

        bool IsPlaying() const;
        void Play();
        void Stop();
    private:
        IXAudio2SourceVoice* sourceVoice = nullptr;
        ObjectPtr<AudioClip> audioClip;
        bool isPlaying = false;

        void OnBufferStart(void* pBufferContext) noexcept override;
        void OnBufferEnd(void* pBufferContext) noexcept override;
        void OnVoiceProcessingPassEnd() noexcept override;
        void OnVoiceProcessingPassStart(UINT32 BytesRequired) noexcept override;
        void OnStreamEnd() noexcept override;
        void OnLoopEnd(void* pBufferContext) noexcept override;
        void OnVoiceError(void* pBufferContext, HRESULT Error) noexcept override;
    };
}
