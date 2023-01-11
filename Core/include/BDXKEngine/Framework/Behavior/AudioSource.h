#pragma once
#include "BDXKEngine/Framework/Core/Component.h"
#include "BDXKEngine/Platform/Audio/AudioClip/AudioClip.h"
#include "BDXKEngine/Platform/Audio/AudioSourceVoice/AudioSourceVoice.h"
#include "Core/Behavior.h"

namespace BDXKEngine
{
    class AudioSource : public Behavior, public EnableHandler, public DisableHandler, public UpdateHandler
    {
    public:
        const ObjectPtr<AudioClip>& GetClip() const;
        float GetVolume() const;
        bool GetLoop() const;

        void SetClip(const ObjectPtr<AudioClip>& clip);
        void SetVolume(float volume);
        void SetLoop(bool loop);

        bool IsPlaying() const;

        void Play();
        void Pause() const;
        void Stop();
    private:
        ObjectPtr<AudioClip> clip;
        float volume = 1;
        bool loop = false;
        bool playOnEnable = true;

        AudioSourceVoice* audioSourceVoice = nullptr;

        void OnEnable() override;
        void OnDisable() override;
        void OnUpdate() override;
        void Transfer(Transferer& transferer) override;
    };

    CustomReflection(AudioSource)
}
