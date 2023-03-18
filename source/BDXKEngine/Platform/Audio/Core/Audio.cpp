#include "Audio.h"
#include <exception>

namespace BDXKEngine
{
    CComPtr<IXAudio2> Audio::xAudio = nullptr;
    IXAudio2MasteringVoice* Audio::masteringVoice = nullptr;

    void Audio::Initialize()
    {
        HRESULT result = CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED);
        if (FAILED(result))throw std::exception("初始化COM失败");

        {
            result = XAudio2Create(&xAudio.p, 0,XAUDIO2_DEFAULT_PROCESSOR);
            if (FAILED(result))throw std::exception("创建声音引擎失败");
            result = xAudio->CreateMasteringVoice(&masteringVoice);
            if (FAILED(result))throw std::exception("创建主语音失败"); //封装了音频设备接口，必须要有这个才能使用音频功能
        }

        CoUninitialize();
    }
    CComPtr<IXAudio2> Audio::GetXAudio()
    {
        return xAudio;
    }
}
