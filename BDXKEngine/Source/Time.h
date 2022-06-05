#pragma once
#include <chrono>

class Time
{
public:
	static float GetDeltaTime();
	static float GetRealtimeSinceStartup();
protected:
	static void Initialize();
	static void BeginFrame();
	static void EndFrame();
private:
	static long startTime;
	static long frameTime;
	static float deltaTime;
};

