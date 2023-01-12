#include "AudioSourceVoice.h"

#include <thread>

namespace BDXKEngine
{
    AudioSourceVoice::AudioSourceVoice(const ObjectPtr<AudioClip>& audioClip)
    {
        HRESULT result = Audio::GetXAudio()->CreateSourceVoice(&sourceVoice, &audioClip->GetWaveFormatex(), 0, 2, this);
        if (FAILED(result))throw std::exception("创建声音源失败");
        result = sourceVoice->SubmitSourceBuffer(&audioClip->GetBuffer());
        if (FAILED(result))throw std::exception("提交声音数据失败");

        this->audioClip = audioClip;
    }
    AudioSourceVoice::~AudioSourceVoice()
    {
        sourceVoice->DestroyVoice();
    }

    void AudioSourceVoice::SetVolume(float volume) const
    {
        sourceVoice->SetVolume(volume);
    }

    bool AudioSourceVoice::IsPlaying() const
    {
        return isPlaying;
    }
    void AudioSourceVoice::Play()
    {
        const HRESULT result = sourceVoice->Start();
        if (FAILED(result))throw std::exception("播放声音失败");
        isPlaying = true;
    }
    void AudioSourceVoice::Stop()
    {
        if (isPlaying == false)
            return;

        const HRESULT result = sourceVoice->Stop();
        if (FAILED(result))throw std::exception("停止声音失败");
        isPlaying = false;
    }

    void AudioSourceVoice::OnBufferStart(void* pBufferContext) noexcept
    {

    }
    void AudioSourceVoice::OnBufferEnd(void* pBufferContext) noexcept
    {
   
    }
    void AudioSourceVoice::OnVoiceProcessingPassEnd() noexcept
    {
    }
    void AudioSourceVoice::OnVoiceProcessingPassStart(UINT32 BytesRequired) noexcept
    {
    }
    void AudioSourceVoice::OnStreamEnd() noexcept
    {
        sourceVoice->Stop();
        sourceVoice->SubmitSourceBuffer(&audioClip->GetBuffer());
        isPlaying = false;
    }
    void AudioSourceVoice::OnLoopEnd(void* pBufferContext) noexcept
    {
    }
    void AudioSourceVoice::OnVoiceError(void* pBufferContext, HRESULT Error) noexcept
    {
    }
}
