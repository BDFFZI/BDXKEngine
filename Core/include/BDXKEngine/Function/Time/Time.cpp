#include "Time.h"
#include <chrono>
#include "BDXKEngine/Platform/Window/Window.h"

namespace BDXKEngine
{
    constexpr float unit = 1000;

    long GetSystemTime()
    {
        const auto currentTime = std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::system_clock::now().time_since_epoch());
        return static_cast<long>(currentTime.count());
    }

    int Time::frameCount = 0;
    long Time::startTime = 0;
    long Time::frameTime = 0;
    float Time::deltaTime = 0;

    float Time::GetRealtimeSinceStartup()
    {
        return static_cast<float>(GetSystemTime() - startTime) / unit;
    }
    float Time::GetDeltaTime()
    {
        return deltaTime;
    }
    int Time::GetFrameCount()
    {
        return frameCount;
    }
    void Time::RenewFrame()
    {
        EndFrame();
        BeginFrame();
    }

    void Time::Initialize()
    {
        startTime = GetSystemTime();
        BeginFrame();

        Window::AddRenewEvent([]()
        {
            EndFrame();
            frameCount++;
            BeginFrame();
        });
    }

    void Time::BeginFrame()
    {
        frameTime = GetSystemTime();
    }
    void Time::EndFrame()
    {
        deltaTime = static_cast<float>(GetSystemTime() - frameTime) / unit;
    }
}
