#include "Time.h"
#include <chrono>

constexpr float unit = 1000;

long GetSystemTime()
{
	auto currentTime = std::chrono::duration_cast<std::chrono::milliseconds>(
		std::chrono::system_clock::now().time_since_epoch());
	return (long)currentTime.count();
}


long Time::startTime = 0;
long Time::frameTime = 0;
float Time::deltaTime = 0;

void Time::Initialize()
{
	Time::startTime = GetSystemTime();
}

float Time::GetRealtimeSinceStartup()
{
	return (GetSystemTime() - startTime) / unit;
}

void Time::BeginFrame()
{
	Time::frameTime = GetSystemTime();
}

void Time::EndFrame()
{
	Time::deltaTime = (GetSystemTime() - Time::frameTime) / unit;
}

float Time::GetDeltaTime()
{
	return deltaTime;
}