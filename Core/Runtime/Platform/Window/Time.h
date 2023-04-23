#pragma once

namespace BDXKEngine
{
    class Window;

    class Time
    {
        friend Window;

    public:
        static float GetTimeScale();
        static float GetTimeReal();
        static float GetTime();
        static float GetDeltaTime();
        static int GetFrameCount();

        static void ResetFrame();

    private:
        inline static float timeScale = 1;
        inline static float time;
        inline static float deltaTime;
        inline static int frameCount = -1;
        inline static float lastTimeReal = 0;

        static void Update();
    };
}
