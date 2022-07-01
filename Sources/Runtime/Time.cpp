#include "Time.h"


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

float Time::GetRealtimeSinceStartup()
{
	return (GetSystemTime() - startTime) / unit;
}
float Time::GetDeltaTime()
{
	return deltaTime;
}

void Time::Initialize(Window* window)
{
	Time::startTime = GetSystemTime();
	window->AddMessageListener(OnWindowMessage);
}

void Time::BeginFrame()
{
	Time::frameTime = GetSystemTime();
}
void Time::EndFrame()
{
	Time::deltaTime = (GetSystemTime() - Time::frameTime) / unit;
}

void Time::OnWindowMessage(Window* window, UINT messageSign, WPARAM wparameter, LPARAM lparameter)
{
	switch (messageSign)
	{
	case WM_PAINT:
	{
		Time::EndFrame();
		Time::BeginFrame();
		break;
	}
	}
}

