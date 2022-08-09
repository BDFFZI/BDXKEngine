#include "Time.h"

namespace BDXKEngine {
	constexpr float unit = 1000;

	long GetSystemTime()
	{
		auto currentTime = std::chrono::duration_cast<std::chrono::milliseconds>(
			std::chrono::system_clock::now().time_since_epoch());
		return (long)currentTime.count();
	}

	int Time::frameCount = 0;
	long Time::startTime = 0;
	long Time::frameTime = 0;
	float Time::deltaTime = 0;

	float Time::GetRealtimeSinceStartup()
	{
		return (GetSystemTime() - startTime) / unit;
	}
	float Time::GetDeltaTime()
	{
		return deltaTime;
	}
	float Time::GetFrameCount()
	{
		return frameCount;
	}

	void Time::Initialize(Window* window)
	{
		Time::startTime = GetSystemTime();
		Time::BeginFrame();

		window->AddRenewEvent([]() {
			Time::EndFrame();
			frameCount++;
			Time::BeginFrame();
			});
	}

	void Time::BeginFrame()
	{
		Time::frameTime = GetSystemTime();
	}
	void Time::EndFrame()
	{
		Time::deltaTime = (GetSystemTime() - Time::frameTime) / unit;
	}
}
