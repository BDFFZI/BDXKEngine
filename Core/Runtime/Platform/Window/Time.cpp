#include "Time.h"
#include <GLFW/glfw3.h>

namespace BDXKEngine
{
    float Time::GetTimeScale()
    {
        return timeScale;
    }
    float Time::GetTimeReal()
    {
        return static_cast<float>(glfwGetTime());
    }
    float Time::GetTime()
    {
        return time;
    }
    float Time::GetDeltaTime()
    {
        return deltaTime;
    }
    int Time::GetFrameCount()
    {
        return frameCount;
    }
    void Time::ResetFrame()
    {
        lastTimeReal = GetTimeReal();
    }

    void Time::Update()
    {
        const float timeReal = GetTimeReal();
        deltaTime = timeReal - lastTimeReal;
        deltaTime *= timeScale;
        time += deltaTime;
        lastTimeReal = timeReal;
        frameCount++;
    }
}
