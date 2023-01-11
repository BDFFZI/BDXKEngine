#include "AudioClip.h"

namespace BDXKEngine
{
    ObjectPtr<AudioClip> AudioClip::Create(const std::vector<char>& fmt, const std::vector<char>& data)
    {
        ObjectPtr clip = new AudioClip();
        clip->fmt = fmt;
        clip->data = data;
        Instantiate(clip);
        return clip;
    }
    
    WAVEFORMATEX& AudioClip::GetWaveFormatex()
    {
        return waveformatex;
    }
    XAUDIO2_BUFFER& AudioClip::GetBuffer()
    {
        return buffer;
    }

    void AudioClip::Transfer(Transferer& transferer)
    {
        Object::Transfer(transferer);

        TransferFieldInfo(fmt);
        TransferFieldInfo(data);
    }
    void AudioClip::Awake()
    {
        Object::Awake();

        memcpy_s(&waveformatex, sizeof waveformatex, fmt.data(), fmt.size());
        buffer.AudioBytes = static_cast<UINT32>(data.size());
        buffer.pAudioData = reinterpret_cast<unsigned char*>(data.data());
        buffer.Flags = XAUDIO2_END_OF_STREAM;
    }
}
