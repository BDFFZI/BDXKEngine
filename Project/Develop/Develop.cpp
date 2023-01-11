#include <fstream>
#include <xaudio2.h>
#include <atlbase.h>
#include <iostream>

#include "BDXKEngine/Platform/Audio/AudioClip/AudioClipImport.h"
#include "BDXKEngine/Platform/Audio/AudioSourceVoice/AudioSourceVoice.h"
#include "BDXKEngine/Platform/Audio/Core/Audio.h"
using namespace BDXKEngine;

int main(int argc, char* argv[])
{
    setlocale(LC_ALL, "zh-CN.UTF-8");

    Audio::Initialize();
    AudioSourceVoice sourceVoice = {AudioClipImport::Wav(R"(C:\Users\BDFFZI\Desktop\Shoot.wav)")};

    sourceVoice.Play();

    while (true)
    {
        // if (sourceVoice.IsPlaying() == false)
        //     sourceVoice.Play();
    }

    printf("");
}
