#pragma once
#include <atlcomcli.h>
#include <xaudio2.h>

namespace BDXKEngine
{
    class Audio
    {
    public:
        static void Initialize();

        static CComPtr<IXAudio2> GetXAudio();
    private:
        static CComPtr<IXAudio2> xAudio;
        static IXAudio2MasteringVoice* masteringVoice;
    };
}
