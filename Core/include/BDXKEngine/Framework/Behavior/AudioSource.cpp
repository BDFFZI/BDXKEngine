﻿#include "AudioSource.h"

namespace BDXKEngine
{
    const ObjectPtr<AudioClip>& AudioSource::GetClip() const
    {
        return clip;
    }
    float AudioSource::GetVolume() const
    {
        return volume;
    }
    bool AudioSource::GetLoop() const
    {
        return loop;
    }

    void AudioSource::SetClip(const ObjectPtr<AudioClip>& clip)
    {
        this->clip = clip;
        if (clip == nullptr)
            Stop();
        else
        {
            if (IsPlaying())
            {
                Stop();
                Play();
            }
            else Stop();
        }
    }
    void AudioSource::SetVolume(float volume)
    {
        this->volume = volume;
        if (IsPlaying())
            audioSourceVoice->SetVolume(volume);
    }
    void AudioSource::SetLoop(bool loop)
    {
        this->loop = loop;
    }
    bool AudioSource::IsPlaying() const
    {
        if (audioSourceVoice == nullptr)
            return false;
        return audioSourceVoice->IsPlaying();
    }

    void AudioSource::Play()
    {
        if (clip == nullptr)return;

        if (audioSourceVoice == nullptr)
            audioSourceVoice = new AudioSourceVoice(clip);

        audioSourceVoice->SetVolume(volume);
        audioSourceVoice->Play();
    }
    void AudioSource::Pause() const
    {
        if (audioSourceVoice == nullptr)return;

        audioSourceVoice->Stop();
    }
    void AudioSource::Stop()
    {
        if (audioSourceVoice == nullptr)return;

        audioSourceVoice->Stop();
        delete audioSourceVoice;
        audioSourceVoice = nullptr;
    }

    void AudioSource::OnEnable()
    {
        if (playOnEnable)
            Play();
    }
    void AudioSource::OnDisable()
    {
        Stop();
    }
    void AudioSource::OnUpdate()
    {
        if (loop && IsPlaying() == false)
            Play();
    }
    void AudioSource::Transfer(Transferer& transferer)
    {
        Component::Transfer(transferer);

        TransferFieldInfo(clip);
        TransferFieldInfo(volume);
        TransferFieldInfo(loop);
        TransferFieldInfo(playOnEnable);
    }
}
